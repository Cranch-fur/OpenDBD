/* 00000000 */ struct /*VFT*/ DBDBaseState_vtbl // sizeof=0x48
/* 00000000 */ {
/* 00000000 */     void (__fastcall *~IStateCollection)(IStateCollection *this);
/* 00000008 */     bool (__fastcall *AddState)(IStateCollection *this, TSharedPtr<BaseState,0>);
/* 00000010 */     bool (__fastcall *AddAndSetAsDefaultState)(IStateCollection *this, TSharedPtr<BaseState,0>);
/* 00000018 */     TSharedPtr<BaseState,0> *(__fastcall *GetState)(IStateCollection *this, TSharedPtr<BaseState,0> *result, const FString *);
/* 00000020 */     TSharedPtr<BaseState,0> *(__fastcall *GetDefaultSubState)(BaseState *this, TSharedPtr<BaseState,0> *result);
/* 00000028 */     TSharedPtr<BaseState,0> *(__fastcall *FindTransitionState)(BaseState *this, TSharedPtr<BaseState,0> *result, UObject *);
/* 00000030 */     void (__fastcall *OnEnter)(BaseState *this, UObject *);
/* 00000038 */     void (__fastcall *OnTick)(BaseState *this, UObject *, float);
/* 00000040 */     void (__fastcall *OnExit)(BaseState *this, UObject *);
/* 00000048 */ };
