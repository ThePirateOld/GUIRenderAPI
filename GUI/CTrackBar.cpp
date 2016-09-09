#include "CGUI.h"

#define TRACKBAR_THUMBSIZE 10

CTrackBarHorizontal::CTrackBarHorizontal ( CDialog *pDialog )
{
	SetControl ( pDialog, TYPE_TRACKBARHORIZONTAL );

	m_nMin = 0;
	m_nMax = 1;
	m_nValue = 1;
}

//--------------------------------------------------------------------------------------
void CTrackBarHorizontal::Draw ( void )
{
	if ( !m_bVisible )
		return;

	CControl::Draw ();

	SControlRect rBox = m_rBoundingBox;
	rBox.pos.SetY ( rBox.pos.GetY () + m_rBoundingBox.size.cy / 2 - 2 );

	rBox.size.cy = 5;
	m_pDialog->DrawBox ( rBox, m_sControlColor.d3dColorBoxBack, m_sControlColor.d3dColorOutline, m_bAntAlias );
	
	rBox.size.cx = m_nButtonX;
	m_pDialog->DrawBox ( rBox, m_sControlColor.d3dColorBoxSel, m_sControlColor.d3dColorOutline, m_bAntAlias );

	m_pDialog->DrawBox ( m_rThumb, m_sControlColor.d3dColorBox [ m_eState ], m_sControlColor.d3dColorOutline, m_bAntAlias );
}

bool CTrackBarHorizontal::OnKeyDown ( WPARAM wParam )
{
	if ( !CanHaveFocus () )
		return false;

	switch ( wParam )
	{
		case VK_HOME:
			SetValue ( m_nMin );
			return true;

		case VK_END:
			SetValue ( m_nMax );
			return true;

		case VK_LEFT:
		case VK_DOWN:
			SetValue ( m_nValue - 1 );
			return true;

		case VK_RIGHT:
		case VK_UP:
			SetValue ( m_nValue + 1 );
			return true;

		case VK_NEXT:
			SetValue ( m_nValue - ( 10 > ( m_nMax - m_nMin ) / 10 ? 10 : ( m_nMax - m_nMin ) / 10 ) );
			return true;

		case VK_PRIOR:
			SetValue ( m_nValue + ( 10 > ( m_nMax - m_nMin ) / 10 ? 10 : ( m_nMax - m_nMin ) / 10 ) );
			return true;
	}

	return false;
}

bool CTrackBarHorizontal::OnMouseButtonDown ( sMouseEvents e )
{
	if ( !CanHaveFocus () )
		return false;

	if ( e.eButton == sMouseEvents::LeftButton )
	{
		if ( m_rThumb.InControlArea ( e.pos ) )
		{
			// Pressed while inside the control
			m_bPressed = true;

			m_nDragOffset = e.pos.GetX () - m_rThumb.pos.GetX ();

			if ( m_pParent && !m_bHasFocus )
				m_pParent->SetFocussedControl ( this );

			return true;
		}

		if ( m_rBoundingBox.InControlArea ( e.pos ) )
		{
			m_nDragOffset = 0;

			// Pressed while inside the control
			m_bPressed = true;

			if ( m_pParent && !m_bHasFocus )
				m_pParent->SetFocussedControl ( this );

			if ( e.pos.GetX () > m_rThumb.pos.GetX () )
			{
				SetValue ( m_nValue + 1 );
				return true;
			}

			if ( e.pos.GetX () < m_rThumb.pos.GetX () )
			{
				SetValue ( m_nValue - 1 );
				return true;
			}
		}
	}

	return false;
}

bool CTrackBarHorizontal::OnMouseButtonUp ( sMouseEvents e )
{
	if ( m_bPressed )
	{
		m_bPressed = false;
		SendEvent ( EVENT_CONTROL_SELECT, m_nValue );

		if ( m_pParent )
			m_pParent->ClearControlFocus ();

		return true;
	}

	return false;
}

bool CTrackBarHorizontal::OnMouseMove ( CPos pos )
{
	if ( m_bPressed )
	{
		SetValue ( ValueFromPos ( pos.GetX () - m_rBoundingBox.pos.GetX () - m_nDragOffset ) );
		return true;
	}

	return false;
}

bool CTrackBarHorizontal::OnMouseWheel ( int zDelta )
{
	SetValue ( m_nValue - zDelta );
	return true;
}

//--------------------------------------------------------------------------------------
int CTrackBarHorizontal::ValueFromPos ( int iX )
{
	float fValuePerPixel = ( float ) ( m_nMax - m_nMin ) / ( m_rBoundingBox.size.cx - TRACKBAR_THUMBSIZE );
	return ( int ) ( 0.5 + m_nMin + fValuePerPixel * iX );
}

//--------------------------------------------------------------------------------------
void CTrackBarHorizontal::SetRange ( int nMin, int nMax )
{
	m_nMin = nMin;
	m_nMax = nMax;

	SetValue ( m_nValue );
}

//--------------------------------------------------------------------------------------
void CTrackBarHorizontal::SetValue ( int nValue )
{
	// Clamp to range
	nValue = __max ( m_nMin, nValue );
	nValue = __min ( m_nMax, nValue );

	if ( nValue == m_nValue )
		return;

	SendEvent ( EVENT_CONTROL_SELECT, m_nValue = nValue );
}

//--------------------------------------------------------------------------------------
void CTrackBarHorizontal::UpdateRects ( void )
{
	CControl::UpdateRects ();

	m_rThumb = m_rBoundingBox;
	m_rThumb.pos.SetX ( m_rThumb.pos.GetX () + m_nButtonX );
	m_rThumb.size.cx = TRACKBAR_THUMBSIZE;

	m_nButtonX = ( int ) ( ( m_nValue - m_nMin ) * ( float ) ( m_rBoundingBox.size.cx - TRACKBAR_THUMBSIZE ) / ( m_nMax - m_nMin ) );
}

//--------------------------------------------------------------------------------------
bool CTrackBarHorizontal::ContainsRect ( CPos pos )
{
	if ( !CanHaveFocus () )
		return false;

	return ( m_rBoundingBox.InControlArea ( pos ) ||
			 m_rThumb.InControlArea ( pos ) );
}
