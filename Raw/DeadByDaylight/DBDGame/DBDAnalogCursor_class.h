class __base(FAnalogCursor, 0) FDBDAnalogCursor
{
/* 0x00 */    struct FDBDAnalogCursor: :VTable* vtable;
/* 0x08 */    __inherited struct FVector2D FAnalogCursor: :CurrentSpeed;
/* 0x10 */        __inherited struct FVector2D FAnalogCursor: :CurrentOffset;
/* 0x18 */        __inherited float FAnalogCursor::Acceleration;
/* 0x1c */        __inherited float FAnalogCursor::MaxSpeedX;
/* 0x20 */        __inherited float FAnalogCursor::MaxSpeedY;
/* 0x24 */        __inherited uint8_t FAnalogCursor::AreButtonsInverted;
/* 0x28 */        __inherited float FAnalogCursor::StickySlowdown;
/* 0x2c */        __inherited float FAnalogCursor::DeadZone;
/* 0x30 */        __inherited enum AnalogCursorMode: :Type FAnalogCursor::Mode;
/* 0x34 */        __inherited struct FVector2D FAnalogCursor: :AnalogValues[0x2];
/* 0x44 */        __padding char _44[4];
/* 0x48 */        uint8_t IsUsingGamePad;
/* 0x49 */        uint8_t IsSticky;
/* 0x4c */        float _cursorDefaultSpeed;
/* 0x50 */        float _cursorStickySpeed;
/* 0x54 */        float _cursorStickIgnore;
/* 0x58 */        class UDBDGameInstance* _gameInstance;
/* 0x60 */        uint32_t _currentUserIndex;
/* 0x64 */        __padding char _64[4];
/* 0x68 */        struct FKeyEvent _lastKeyEvent;
/* 0xa8 */        uint8_t _isLockedOnVertical;
/* 0xa9 */        uint8_t _isLockedOnHorizontal;
/* 0xaa */        __padding char _aa[6];
/* 0xb0 */
};
