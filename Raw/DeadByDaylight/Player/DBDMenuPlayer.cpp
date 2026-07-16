// starts at line 16
ADBDMenuPlayer::ADBDMenuPlayer()
{
    // Initialize default class properties
    this->RoleSelected = false;
    this->CharacterIndex = -1;
    
    // Initialize enum to its default state
    this->_handPosition = EItemHandPosition::None;    
    this->_equippedItemName = NAME_None;

    // Enable the primary tick function for this actor
    this->PrimaryActorTick.bCanEverTick = true;

    // Create the Skeletal Mesh Component for items
    this->ItemMesh = this->CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
    
    // Attach the created item mesh to the character's base mesh
    if (this->ItemMesh != nullptr)
    {
        this->ItemMesh->SetupAttachment(this->Mesh, NAME_None);
    }

    // Bind the wrapped destroyed callback to the actor's OnDestroyed dynamic multicast delegate
    this->OnDestroyed.AddDynamic(this, &ADBDMenuPlayer::WrappedOnDestroyed);
}

// starts at line 28
void ADBDMenuPlayer::BeginPlay()
{
    Super::BeginPlay();
}

// starts at line 35
void ADBDMenuPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// starts at line 41
void ADBDMenuPlayer::BeginDestroySequence_Implementation()
{
    // Disable collision for this actor to prevent it from interacting with the world during the destruction process
    this->SetActorEnableCollision(false);

    // Call the standard Unreal Engine Destroy function to remove the actor from the game world
    // bNetForce is set to false, bShouldModifyLevel is set to true
    this->Destroy(false, true);
}

// starts at line 48
void ADBDMenuPlayer::WrappedOnDestroyed(AActor* DestroyedActor)
{
    // Check if the custom non-dynamic destroyed delegate has any bound functions
    // Custom multicast delegate OnDestroyedNonDynamic
    if (this->OnDestroyedNonDynamic.IsBound() == true)
    {
        // Broadcast the delegate to trigger all bound listener functions
        this->OnDestroyedNonDynamic.Broadcast();
    }
}

// starts at line 56
void ADBDMenuPlayer::EquipItem(FName itemToEquip)
{
    FName ItemID = itemToEquip;
    UGameInstance* GameInstance = this->GetGameInstance();

    // Verify that the GameInstance is valid and is of type UDBDGameInstance
    if (GameInstance == nullptr || GameInstance->IsA(UDBDGameInstance::StaticClass()) == false)
    {
        GameInstance = nullptr;
    }

    USkeletalMeshComponent* CurrentItemMesh = this->ItemMesh;

    // Ensure the mesh component and game instance are fully valid before proceeding
    if (IsValid(CurrentItemMesh) == true && GameInstance != nullptr && IsValid(GameInstance) == true)
    {
        if (ItemID != this->_equippedItemName)
        {
            this->_equippedItemName = ItemID;
            
            // Assuming _handPosition takes a 0-initialized enum state
            this->_handPosition = EItemHandPosition::None;

            // Check if the item slot is explicitly locked or empty
            // LoadoutSlotDefaultValue namespace/class
            if (ItemID == LoadoutSlotDefaultValue::LOCKED || ItemID == LoadoutSlotDefaultValue::EMPTY || ItemID == NAME_None)
            {
                // Virtual call to USkinnedMeshComponent::SetSkeletalMesh at offset 0x780
                CurrentItemMesh->SetSkeletalMesh(nullptr, true);
                return;
            }

            // Downcasting to custom game instance to access DesignTunables
            UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
            
            // UDBDDesignTunables property offset access
            FItemData* ItemData = DBDGameInstance->DesignTunables->GetItemData(ItemID, true);

            if (ItemData != nullptr)
            {
                // Attempt to resolve the soft object pointer/asset pointer directly
                USkeletalMesh* MeshToEquip = ItemData->ItemMesh.Get();

                if (MeshToEquip == nullptr)
                {
                    // Fallback to synchronous loading if the mesh is not yet in memory
                    // StreamableManager accessed from custom game instance
                    FStreamableManager& AssetLoader = DBDGameInstance->AssetLoader;
                    UObject* LoadedObject = AssetLoader.SynchronousLoad(ItemData->ItemMesh.ToStringReference());

                    if (LoadedObject != nullptr && LoadedObject->IsA(USkeletalMesh::StaticClass()) == true)
                    {
                        MeshToEquip = Cast<USkeletalMesh>(LoadedObject);
                    }
                }

                if (MeshToEquip != nullptr)
                {
                    this->_handPosition = ItemData->HandPosition;
                    
                   // Virtual call to USkinnedMeshComponent::SetSkeletalMesh at offset 0x780
                    CurrentItemMesh->SetSkeletalMesh(MeshToEquip, true);
                    return;
                }

                // If mesh fails to load or is invalid, log and send analytics
                // GameFlow log category
                if (GameFlow.Verbosity >= 2)
                {
                    UE_LOG(GameFlow, Log, TEXT("Trying to equip an item [%s] that has no mesh."), *ItemID.ToString());
                }

                TArray<FAnalyticsEventAttribute> AnalyticsAttributes;
                AnalyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("Failure_szni"), TEXT("Invalid mesh")));
                
                // AnalyticsConversion namespace/class
                FString ItemEquipString = AnalyticsConversion::ToString(*ItemID.ToString());
                AnalyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("ItemToEquip_szni"), ItemEquipString));

                // UBHVRAnalytics static method
                UBHVRAnalytics::RecordEvent(TEXT("ADBDMenuPlayer.EquipItem"), AnalyticsAttributes);

                // Virtual call to USkinnedMeshComponent::SetSkeletalMesh at offset 0x780
                CurrentItemMesh->SetSkeletalMesh(nullptr, true);
                return;
            }
            else
            {
                // If ItemData is missing in design tunables, log and send analytics
                // GameFlow log category
                if (GameFlow.Verbosity >= 2)
                {
                    UE_LOG(GameFlow, Log, TEXT("Trying to equip an item [%s] that is not defined in the design tunables."), *ItemID.ToString());
                }

                TArray<FAnalyticsEventAttribute> AnalyticsAttributes;
                AnalyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("Failure_szni"), TEXT("Invalid item")));
                
                // AnalyticsConversion namespace/class
                FString ItemEquipString = AnalyticsConversion::ToString(*ItemID.ToString());
                AnalyticsAttributes.Add(FAnalyticsEventAttribute(TEXT("ItemToEquip_szni"), ItemEquipString));

                // UBHVRAnalytics static method
                UBHVRAnalytics::RecordEvent(TEXT("ADBDMenuPlayer.EquipItem"), AnalyticsAttributes);

                // Virtual call to USkinnedMeshComponent::SetSkeletalMesh at offset 0x780
                CurrentItemMesh->SetSkeletalMesh(nullptr, true);
                return;
            }
        }
    }
}

// starts at line 103
void ADBDMenuPlayer::UpdateCustomizationMesh(TArray<FName> customization, bool isVfxVisible)
{
    // Retrieve the Game Instance from the current actor
    UGameInstance* GameInstance = this->GetGameInstance();

    // Verify that the GameInstance is valid and is of the expected custom type
    // UDBDGameInstance is a custom subclass of UGameInstance
    if (GameInstance == nullptr || GameInstance->IsA(UDBDGameInstance::StaticClass()) == false)
    {
        GameInstance = nullptr;
    }

    // Retrieve the customized skeletal mesh component attached to this actor
    // UCustomizedSkeletalMesh is a custom component class
    UCustomizedSkeletalMesh* CustomizedSkeletalMesh = Cast<UCustomizedSkeletalMesh>(this->GetComponentByClass(UCustomizedSkeletalMesh::StaticClass()));

    // Check if both the Game Instance and the customized mesh component are valid and not marked as unreachable
    if (IsValid(GameInstance) == true && IsValid(CustomizedSkeletalMesh) == true)
    {
        // Iterate through the provided array of customization item IDs
        for (int32 Index = 0; Index < customization.Num(); ++Index)
        {
            FName CustomizationItemID = customization[Index];

            // Downcast GameInstance to access specific properties like DesignTunables
            UDBDGameInstance* DBDGameInstance = Cast<UDBDGameInstance>(GameInstance);
            
            // Retrieve the customization item data from the design tunables
            // DesignTunables and FCustomizationItemData are custom elements
            FCustomizationItemData* CustomizationItemData = DBDGameInstance->DesignTunables->GetCustomizationItemData(CustomizationItemID, true);

            if (CustomizationItemData != nullptr)
            {
                // Switch the mesh part on the component using the retrieved category and the item ID
                // SwitchMesh is a custom method of UCustomizedSkeletalMesh
                CustomizedSkeletalMesh->SwitchMesh(CustomizationItemData->PartCategory, CustomizationItemID, isVfxVisible);
            }
        }
    }
}
