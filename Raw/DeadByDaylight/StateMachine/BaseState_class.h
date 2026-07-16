class __base(IStateCollection, 0) BaseState
{
/*0x0000*/  struct BaseState::VTable* vtable;
/*0x0008*/  struct TWeakObjectPtr<UStateMachine, FWeakObjectPtr> _stateMachine;
/*0x0010*/  class BaseState* _parentState;
/*0x0018*/  class FString _name;
/*0x0028*/  class TArray<TSharedPtr<BaseState, 0>, FDefaultAllocator> _subStates;
/*0x0038*/  class TSharedPtr<BaseState, 0> _defaultSubState;
/*0x0048*/  uint16_t _id;
/*0x004c*/  uint32_t _numTickInState;
};
