#pragma once

#include "CPos.h"

class CDialog;

enum E_EVENT_CONTROL
{
	EVENT_CONTROL_KEY_DOWN,
	EVENT_CONTROL_CLICKED,
	EVENT_CONTROL_GOT_FOCUS,
	EVENT_CONTROL_LOST_FOCUS,
	EVENT_CONTROL_CHANGE,
	EVENT_CONTROL_SELECT,
	EVENT_CONTROL_MOUSE_OVER,
	EVENT_CONTROL_MOUSE_OUT,
	EVENT_CONTROL_END,
	EVENT_CONTROL_DBCLICK
};

typedef void ( __cdecl *tAction )( CControl*, E_EVENT_CONTROL, int );

struct SControlRect
{
	CPos pos;
	SIZE size;

	SControlRect ( void )
	{}

	SControlRect ( CPos pos )
	{
		this->pos = pos;
	}

	SControlRect ( int iX, int iY )
	{
		this->pos.SetX ( iX );
		this->pos.SetY ( iY );
	}

	SControlRect ( SIZE size )
	{
		this->size = size;
	}

	SControlRect ( CPos pos, SIZE size )
	{
		this->pos = pos;
		this->size = size;
	}

	SControlRect ( RECT rect )
	{
		this->pos.SetX ( rect.left );
		this->pos.SetY ( rect.top );
		this->size.cx =  rect.right- rect.left;
		this->size.cy =  rect.bottom- rect.top;
	}

	BOOL InControlArea ( CPos pos )
	{
		return ( pos.GetX () >= this->pos.GetX () &&
				 pos.GetX () <= this->pos.GetX () + size.cx &&
				 pos.GetY () >= this->pos.GetY () &&
				 pos.GetY () <= this->pos.GetY () + size.cy );
	}

	RECT GetRect ( void )
	{
		RECT rect;
		SetRect ( &rect, pos.GetX (), pos.GetY (), pos.GetX () + size.cx, pos.GetY () + size.cy );
		return rect;
	}


};

struct SControlColor
{
	enum SControlState
	{
		STATE_NORMAL,
		STATE_MOUSE_OVER,
		STATE_PRESSED,
		STATE_DISABLED
	};

	D3DCOLOR d3dColorWindowTitle;
	D3DCOLOR d3dCOlorWindowBack;

	D3DCOLOR d3dColorBoxSel;
	D3DCOLOR d3dColorBoxBack;
	D3DCOLOR d3dColorBox [ 4 ];

	D3DCOLOR d3dColorOutline;
	D3DCOLOR d3dColorShape;

	D3DCOLOR d3dColorSelectedFont;
	D3DCOLOR d3dColorFont;
};

class CControl
{
public:

	enum EControlType
	{
		TYPE_BUTTON,
		TYPE_CHECKBOX,
		TYPE_RADIOBUTTON,
		TYPE_DROPDOWN,
		TYPE_EDITBOX,
		TYPE_IMAGE,
		TYPE_BOX,
		TYPE_LISTBOX,
		TYPE_SCROLLBARHORIZONTAL,
		TYPE_SCROLLBARVERTICAL,
		TYPE_TRACKBARHORIZONTAL,
		TYPE_TRACKBARVERTICAL,
		TYPE_PROGRESSBARHORIZONTAL,
		TYPE_PROGRESSBARVERTICAL,
		TYPE_LABEL,
		TYPE_TEXTBOX,
		TYPE_LISTVIEW,
		TYPE_TABPANEL,
		TYPE_WINDOW
	};

	void EnterScissorRect ( SControlRect rRect );
	void LeaveScissorRect ( void );

	void SetColor ( SControlColor sColor );
	SControlColor GetColor ( SControlColor sColor );

	void SetControl ( CDialog *pGui, EControlType eType );

	virtual void ClearControlFocus ( void );
	virtual void SetFocussedControl ( CControl *pControl );

	void SetParent ( CControl *pParent );
	CControl *GetParent ( void );

	virtual SIZE GetRealSize ( void );

	void SetAction ( tAction pAction );
	tAction GetAction ( void );

	void SetPos ( int X, int Y );
	void SetPos ( CPos relPos );

	CPos *GetPos ( void );
	CPos *GetUpdatedPos ( void );

	virtual void SetWidth ( int iWidth );
	int GetWidth ( void );

	virtual void SetHeight ( int iHeight );
	int GetHeight ( void );

	virtual void SetSize ( SIZE size );
	virtual void SetSize ( int iWidth, int iHeight );

	void SetMinSize ( int nMin, int nMax );
	void SetMinSize ( SIZE size );
	SIZE GetMinSize ( SIZE size );

	virtual bool IsSizingX ( void );
	virtual bool IsSizingY ( void );

	virtual bool IsSizing ( void );

	virtual bool IsMovingX ( void );
	virtual bool IsMovingY ( void );
	virtual bool IsMoving ( void );

	SIZE GetSize ( void );

	virtual bool CanHaveFocus ( void );
	bool HasFocus ( void );

	virtual void SetText ( SIMPLEGUI_STRING sString, bool = false );
	const SIMPLEGUI_CHAR *GetText ( void );

	void SendMsg ( E_EVENT_CONTROL, int );

	virtual void Draw ( void );

	virtual bool MsgProc ( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool HandleKeyboard ( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool HandleMouse ( UINT uMsg, CPos pos, WPARAM wParam, LPARAM lParam );

	void SetFont ( const SIMPLEGUI_CHAR *szFontName, DWORD dwHeight, bool bBold = false );
	void SetFont ( CD3DFont *pFont );
	CD3DFont *GetFont ( void );

	void SetStateColor ( D3DCOLOR d3dColor, SControlColor::SControlState eState );

	virtual void OnFocusIn ( void );
	virtual void OnFocusOut ( void );

	virtual bool OnMouseOver ( void );
	virtual void OnMouseEnter ( void );
	virtual void OnMouseLeave ( void );

	virtual void OnClickEnter ( void );
	virtual void OnClickLeave ( void );
	virtual	bool OnClickEvent ( void );

	virtual bool OnMouseButtonDown ( CPos pos );
	virtual bool OnMouseButtonUp ( CPos pos );
	virtual bool OnMouseMove ( CPos pos );
	virtual bool OnMouseWheel ( int zDelta );

	virtual bool OnKeyDown ( WPARAM wParam );
	virtual bool OnKeyUp ( WPARAM wParam );
	virtual bool OnKeyCharacter ( WPARAM wParam );

	bool SendEvent ( E_EVENT_CONTROL event, int params );

	void LinkPos ( CPos pos );


	bool IsRelativePos ( void ) { return m_bRelativePosX|| m_bRelativePosY; }
	void SetRelativePosX ( bool bRelative );
	void SetRelativePosY ( bool bRelative );

	void SetRelativeSizeX ( bool bRelative );
	void SetRelativeSizeY ( bool bRelative );

	void SetEnabled ( bool bEnabled = true );
	bool IsEnabled ( void );

	void SetVisible ( bool bVisible = true );
	bool IsVisible ( void );

	void SetEnabledStateColor ( bool bEnable = true );
	void SetAntAlias ( bool bAntAlias = true );

	EControlType GetType ( void );

	virtual void UpdateRects ( void );
	virtual bool ContainsRect ( CPos pos );
protected:

	SIZE m_size;
	SIZE m_minSize;

	SIZE m_realSize;

	SControlRect m_rScissor;
	SControlRect m_rScissorCpy;

	SControlRect m_rContentBox;
	SControlRect m_rBoundingBox;

	CD3DTexture *m_pTexture;
	CD3DFont* m_pFont;
	CControl* m_pParent;
	CDialog *m_pDialog;

	SIMPLEGUI_STRING m_sText;

	SControlColor m_sControlColor;

	EControlType m_eType;
	tAction m_pAction;

	SControlColor::SControlState m_eState;

	CRITICAL_SECTION cs;
	CPos m_pos;
	CPos m_oldPos;

	bool m_bRelativePosX;
	bool m_bRelativePosY;

	bool m_bRelativeSizeX;
	bool m_bRelativeSizeY;

	bool m_bAntAlias;
	bool m_bPressed;
	bool m_bMouseOver;

	bool m_bHasFocus;

	bool m_bEnabled;
	bool m_bVisible;

	bool m_bEnabledStateColor;
};

