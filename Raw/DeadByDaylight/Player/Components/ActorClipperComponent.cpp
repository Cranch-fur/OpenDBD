// starts at line 15
UActorClipperComponent::UActorClipperComponent(const FObjectInitializer& ObjectInitializer)
    : UActorComponent(ObjectInitializer)
{
    // Configure the primary component tick settings.
    this->PrimaryComponentTick.bCanEverTick = true;
    this->PrimaryComponentTick.bStartWithTickEnabled = true;
    
    // Initialize component-specific boolean flags.
    this->_registeredToShapOverlap = false;
    this->_clippingEnabled = true;
}

// starts at line 21
void UActorClipperComponent::BeginDestroy()
{
    // Unregister any active shape overlap events before the component is destroyed
    this->UnregisterFromShapeOverlap();

    // Call the base class implementation
    UActorComponent::BeginDestroy();
}

// starts at line 27
void UActorClipperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    // Check if the owner actor is observed by the local player
    if (this->IsOwnerLocallyObserved() == true)
    {
        // If not already registered to the shape overlap event, register it now
        if (this->_registeredToShapOverlap == false)
        {
            this->RegisterToShapeOverlap();
        }
    }
    // If the owner is not locally observed, but it is currently registered
    else if (this->_registeredToShapOverlap == true)
    {
        // Iterate through all currently clipped actors
        for (auto It = this->_clippedActors.CreateIterator(); It; ++It)
        {
            FClippedActor& ClippedActorData = It.Value();
            
            // Iterate through all display components belonging to the clipped actor
            for (int32 i = 0; i < ClippedActorData.DisplayComponents.Num(); ++i)
            {
                TWeakObjectPtr<USceneComponent>& WeakComponent = ClippedActorData.DisplayComponents[i];
                
                if (WeakComponent.IsValid() == true)
                {
                    USceneComponent* SceneComponent = WeakComponent.Get();
                    
                    // Calling a virtual function on the USceneComponent.
                    // The virtual function is located at offset 0x3C8 (968 decimal) in the VTable.
                    SceneComponent->SetLocallyClipped(false, false);
                }
            }
        }

        // Clear the map of clipped actors and reset its internal state (hash and sparse array logic)
        this->_clippedActors.Empty();

        // Unregister from the shape overlap event since the owner is no longer locally observed
        this->UnregisterFromShapeOverlap();
    }
}

// starts at line 47
void UActorClipperComponent::OnOverlapEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Validate the primitive component that caused the overlap
    if (OtherComp != nullptr)
    {
        // Standard Unreal Engine valid checks on the primitive component
        if (IsValid(OtherComp) == true && this->HasMatchingTag(OtherComp) == true)
        {
            // Attempt to find the actor in the set of currently clipped actors
            const FSetElementId FoundElementId = this->_clippedActors.FindId(OtherActor);

            if (FoundElementId.IsValidId() == true)
            {
                // The actor is already in the map, so fetch the associated clipped data
                FClippedActor* ClippedActorData = &this->_clippedActors[FoundElementId];
                
                if (ClippedActorData != nullptr)
                {
                    // Check if this specific primitive component (OtherComp) is already in the list of colliding primitives for this actor
                    const FSetElementId PrimitiveElementId = ClippedActorData->CollidingPrimitives.FindId(OtherComp);
                    
                    if (PrimitiveElementId.IsValidId() == false)
                    {
                        // Primitive component is not tracked yet, add a weak reference to the colliding primitives set
                        TWeakObjectPtr<UPrimitiveComponent> WeakOtherComp = OtherComp;
                        ClippedActorData->CollidingPrimitives.Add(OtherComp, WeakOtherComp);
                    }
                }
            }
            else
            {
                // The actor is not yet tracked, create a new entry for it
                FClippedActor NewEntry;
                
                // Add the primitive component that caused the overlap to the new entry's colliding primitives
                TWeakObjectPtr<UPrimitiveComponent> WeakOtherComp = OtherComp;
                NewEntry.CollidingPrimitives.Add(OtherComp, WeakOtherComp);

                // Collect all relevant scene components from the actor to track their display state
                this->CollectAllSceneComponents(OtherActor, NewEntry.DisplayComponents);

                // Add the newly created entry to the map of clipped actors
                this->_clippedActors.Add(OtherActor, NewEntry);

                // If clipping is currently enabled, immediately apply the clipping effect to the newly tracked display components
                if (this->_clippingEnabled == true)
                {
                    for (int32 i = 0; i < NewEntry.DisplayComponents.Num(); ++i)
                    {
                        TWeakObjectPtr<USceneComponent>& WeakComponent = NewEntry.DisplayComponents[i];
                        
                        if (WeakComponent.IsValid() == true)
                        {
                            USceneComponent* SceneComponent = WeakComponent.Get();
                            
                            // Calling a virtual function on the UPrimitiveComponent.
                            // The virtual function is located at offset 0x3C8 (968 decimal) in the VTable.
                            SceneComponent->SetLocallyClipped(true, false);
                        }
                    }
                }
            }
        }
    }
}

// starts at line 76
void UActorClipperComponent::OnOverlapExit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
        // Standard Unreal Engine valid checks on the primitive component
        if (IsValid(OtherComp) == true && this->HasMatchingTag(OtherComp) == true)
        {
            // Attempt to find the actor in the set of currently clipped actors
            const FSetElementId FoundElementId = this->_clippedActors.FindId(OtherActor);

            if (FoundElementId.IsValidId() == true)
            {
                // The actor is tracked, so fetch the associated clipped data
                FClippedActor* ClippedActorData = &this->_clippedActors[FoundElementId];

                // Check if this specific primitive component (OtherComp) is in the list of colliding primitives for this actor
                const FSetElementId PrimitiveElementId = ClippedActorData->CollidingPrimitives.FindId(OtherComp);
                
                if (PrimitiveElementId.IsValidId() == true)
                {
                    // Remove the primitive component from the set of colliding primitives
                    ClippedActorData->CollidingPrimitives.Remove(OtherComp);

                    // If the number of removed primitives equals the array max (or internal capacity metric), 
                    // it indicates there are no more valid colliding primitives keeping the actor clipped
                    if (ClippedActorData->CollidingPrimitives.Num() == 0)
                    {
                        // Unhide/restore the display components of the actor
                        this->ShowActor(*ClippedActorData);
                        
                        // Remove the actor completely from the tracked clipped actors map
                        this->_clippedActors.Remove(OtherActor);
                    }
                }
            }
        }
}

// starts at line 99
bool UActorClipperComponent::HasMatchingTag(UPrimitiveComponent* OtherComp) const
{
    // If there are no tags required by this clipper, automatically return true.
    if (this->_tags.Num() == 0)
    {
        return true;
    }

    // Iterate through all the tags defined in this component
    for (int32 i = 0; i < this->_tags.Num(); ++i)
    {
        // Check if the other component possesses the current tag
        if (OtherComp->ComponentHasTag(this->_tags[i]) == true)
        {
            // Return true on the first matching tag
            return true;
        }
    }

    // If no tags match and the required tags array was not empty, return false
    return false;
}

// starts at line 116
bool UActorClipperComponent::IsOwnerLocallyObserved() const
{
    // Retrieve the owner actor
    AActor* OwnerActor = this->GetOwner();
    bool bIsLocallyObserved = false;

    // Validating that the Actor is not currently being destroyed
    if (IsValid(OwnerActor) == true && OwnerActor->IsActorBeingDestroyed() == false)
    {
        ADBDPlayer* DBDPlayer = Cast<ADBDPlayer>(OwnerActor);

        if (IsValid(DBDPlayer) == true && DBDPlayer->IsValidImpl() == true)
        {
            // Calling a virtual function on the DBDPlayer.
            // The virtual function is located at offset 0xDB8 (3512 decimal) in the VTable.
            bIsLocallyObserved = DBDPlayer->IsLocallyObserved();
        }
    }

    return bIsLocallyObserved;
}

// starts at line 130
void UActorClipperComponent::RegisterToShapeOverlap()
{
    // Ensure the Shape property is set
    UShapeComponent* AssociatedShape = this->Shape;
    
    // Perform standard Unreal Engine validity check on the Shape component
    if (IsValid(AssociatedShape) == true)
    {
        // Bind the OnOverlapEnter function to the Shape's OnComponentBeginOverlap delegate
        // The assembly shows a complex string extraction to get the function name for the FName creation,
        // which is standard for the AddDynamic macro expansion in UE.
        AssociatedShape->OnComponentBeginOverlap.AddDynamic(this, &UActorClipperComponent::OnOverlapEnter);
        
        // Bind the OnOverlapExit function to the Shape's OnComponentEndOverlap delegate
        AssociatedShape->OnComponentEndOverlap.AddDynamic(this, &UActorClipperComponent::OnOverlapExit);
        
        // Flag the component as successfully registered to shape overlap events
        this->_registeredToShapOverlap = true;
    }
}

// starts at line 140
void UActorClipperComponent::UnregisterFromShapeOverlap()
{
    // Ensure the Shape property is set
    UShapeComponent* AssociatedShape = this->Shape;

    // Perform standard Unreal Engine validity check on the Shape component
    if (IsValid(AssociatedShape) == true)
    {
        // Unbind the OnOverlapEnter function from the Shape's OnComponentBeginOverlap delegate
        // The assembly shows a complex string extraction to get the function name for the FName creation,
        // which is standard for the RemoveDynamic macro expansion in UE.
        AssociatedShape->OnComponentBeginOverlap.RemoveDynamic(this, &UActorClipperComponent::OnOverlapEnter);
        
        // Unbind the OnOverlapExit function from the Shape's OnComponentEndOverlap delegate
        AssociatedShape->OnComponentEndOverlap.RemoveDynamic(this, &UActorClipperComponent::OnOverlapExit);
        
        // Flag the component as no longer registered to shape overlap events
        this->_registeredToShapOverlap = false;
    }
}

// starts at line 150
void UActorClipperComponent::CollectAllSceneComponents(const AActor* actor, TArray<TWeakObjectPtr<USceneComponent>>& inOutComponents)
{
    // Validating that the Actor is not currently being destroyed.
    if (IsValid(actor) == true && actor->IsActorBeingDestroyed() == false)
    {
        // 1. Recursively process all child actors attached to this actor
        for (int32 i = 0; i < actor->Children.Num(); ++i)
        {
            AActor* ChildActor = actor->Children[i];
            this->CollectAllSceneComponents(ChildActor, inOutComponents);
        }

        // 2. Retrieve all SceneComponents owned directly by this actor (excluding child actor components)
        TArray<USceneComponent*> OwnedComponents;
        actor->GetComponents<USceneComponent>(OwnedComponents, false);

        // 3. Process each owned component
        for (int32 j = 0; j < OwnedComponents.Num(); ++j)
        {
            USceneComponent* SceneComp = OwnedComponents[j];
            
            // The assembly shows a call to a function with the same name but accepting a USceneComponent pointer.
            this->CollectAllSceneComponents(SceneComp, inOutComponents);
        }
    }
}

// starts at line 169
void UActorClipperComponent::CollectAllSceneComponents(const USceneComponent* sceneComponent, TArray<TWeakObjectPtr<USceneComponent>>& inOutComponents)
{
    // Convert the input scene component into a weak object pointer for comparison and storage
    TWeakObjectPtr<USceneComponent> WeakComponent(sceneComponent);

    // Ensure the component is not already present in the array to avoid duplicates
    // The disassembly shows a manual iteration searching for the pointer in the TArray
    bool bAlreadyExists = false;
    for (int32 i = 0; i < inOutComponents.Num(); ++i)
    {
        if (inOutComponents[i] == WeakComponent)
        {
            bAlreadyExists = true;
            break;
        }
    }

    // If the component is unique to the list, add it
    if (bAlreadyExists == false)
    {
        inOutComponents.Add(WeakComponent);
    }

    // Recursively process all components attached to this scene component
    // USceneComponent::AttachChildren is a TArray of USceneComponent pointers
    for (int32 j = 0; j < sceneComponent->AttachChildren.Num(); ++j)
    {
        USceneComponent* ChildComponent = sceneComponent->AttachChildren[j];
        
        // Ensure child component is valid before recursion
        if (ChildComponent != nullptr)
        {
            this->CollectAllSceneComponents(ChildComponent, inOutComponents);
        }
    }
}

// starts at line 189
void UActorClipperComponent::ShowActor(const FClippedActor& clippedActor)
{
    // Iterate through all the display components associated with the clipped actor
    for (int32 i = 0; i < clippedActor.DisplayComponents.Num(); ++i)
    {
        // Get the weak pointer for the current component
        TWeakObjectPtr<USceneComponent> component = clippedActor.DisplayComponents[i];

        // Check if the component still exists and is valid
        if (component.IsValid() == true)
        {
            // Resolve the weak pointer to an actual USceneComponent pointer
            USceneComponent* SceneComp = component.Get();

            // Calling a virtual function on the USceneComponent.
            // The virtual function is located at offset 0x3C8 (968 decimal) in the VTable.
            SceneComp->SetLocallyClipped(false, false);
        }
    }
}

// starts at line 200
void UActorClipperComponent::SetClippingEnabled(bool enabled)
{
    // Check if the requested state is different from the current state
    if (this->_clippingEnabled != enabled)
    {
        // Update the internal state
        this->_clippingEnabled = enabled;

        // If clipping is being disabled, we must restore the visibility of all currently clipped actors
        if (enabled == false)
        {
            // Iterate through all currently tracked clipped actors
            for (auto It = this->_clippedActors.CreateIterator(); It; ++It)
            {
                FClippedActor& ClippedActorData = It.Value();
                
                // Iterate through the stored display components of the clipped actor
                for (int32 i = 0; i < ClippedActorData.DisplayComponents.Num(); ++i)
                {
                    TWeakObjectPtr<USceneComponent>& WeakComponent = ClippedActorData.DisplayComponents[i];
                    
                    if (WeakComponent.IsValid() == true)
                    {
                        USceneComponent* SceneComponent = WeakComponent.Get();

                        // Calling a virtual function on the USceneComponent.
                        // The virtual function is located at offset 0x3C8 (968 decimal) in the VTable.
                        PrimitiveComponent->SetLocallyClipped(false, false);
                    }
                }
            }
        }
    }
}
