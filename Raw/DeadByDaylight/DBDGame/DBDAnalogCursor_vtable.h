/* 00000000 */ struct /*VFT*/ FDBDAnalogCursor_vtbl // sizeof=0x30
/* 00000000 */ {
/* 00000000 */     void(__fastcall * ~FDBDAnalogCursor)(struct FDBDAnalogCursor* this);
/* 00000008 */     void(__fastcall* Tick)(struct FDBDAnalogCursor* this, const float, FSlateApplication*, TSharedRef<ICursor, 0>);
/* 00000010 */     bool(__fastcall * HandleKeyDownEvent)(struct FDBDAnalogCursor* this, FSlateApplication*, const FKeyEvent*);
/* 00000018 */     bool(__fastcall* HandleKeyUpEvent)(struct FDBDAnalogCursor* this, FSlateApplication*, const FKeyEvent*);
/* 00000020 */     bool(__fastcall * HandleAnalogInputEvent)(struct FDBDAnalogCursor* this, FSlateApplication*, const FAnalogInputEvent*);
/* 00000028 */     bool(__fastcall* HandleMouseMoveEvent)(struct FDBDAnalogCursor* this, FSlateApplication*, const FPointerEvent*);
/* 00000030 */ };