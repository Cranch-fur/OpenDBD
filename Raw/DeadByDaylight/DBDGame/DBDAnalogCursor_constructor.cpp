void __fastcall FDBDAnalogCursor::FDBDAnalogCursor(FDBDAnalogCursor* this)
{
	__int16 v2; // [rsp+30h] [rbp+8h]

	FAnalogCursor::FAnalogCursor(this);
	*(_WORD*)&this->IsUsingGamePad = 0;
	this->__vftable = (FDBDAnalogCursor_vtbl*)&FDBDAnalogCursor::vftable;
		this->_cursorDefaultSpeed = 1800.0;
	this->_cursorStickySpeed = 950.0;
	this->_cursorStickIgnore = 2.0;
	this->_lastKeyEvent.ModifierKeys = (FModifierKeysState)(v2 & 0xFE00);
	this->_lastKeyEvent.__vftable = (FKeyEvent_vtbl*)&FKeyEvent::vftable;
		this->_lastKeyEvent.UserIndex = 0;
	this->_lastKeyEvent.bIsRepeat = 0;
	this->_lastKeyEvent.EventPath = 0;
	this->_lastKeyEvent.Key.KeyName.CompositeComparisonValue = 0;
	this->_lastKeyEvent.Key.KeyDetails.Object = 0;
	this->_lastKeyEvent.Key.KeyDetails.SharedReferenceCount.ReferenceController = 0;
	*(_QWORD*)&this->_lastKeyEvent.CharacterCode = 0;
	*(_WORD*)&this->_isLockedOnVertical = 0;
	this->_currentUserIndex = -1;
}