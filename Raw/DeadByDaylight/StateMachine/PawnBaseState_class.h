class __base(DBDBaseState, 0) PawnBaseState
{
/*0x0000*/  struct PawnBaseState::VTable* vtable;
/*0x0008*/  __inherited struct TWeakObjectPtr<UStateMachine, FWeakObjectPtr> DBDBaseState::_stateMachine;
/*0x0010*/  __inherited class BaseState* DBDBaseState::_parentState;
/*0x0018*/  __inherited class FString DBDBaseState::_name;
/*0x0028*/  __inherited class TArray<TSharedPtr<BaseState, 0>, FDefaultAllocator> DBDBaseState::_subStates;
/*0x0038*/  __inherited class TSharedPtr<BaseState, 0> DBDBaseState::_defaultSubState;
/*0x0048*/  __inherited uint16_t DBDBaseState::_id;
/*0x004c*/  __inherited uint32_t DBDBaseState::_numTickInState;
};
