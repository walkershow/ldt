#include "ListContainerElementUIex.h"

CListContainerElementUIex::CListContainerElementUIex(void)
{
}



void CListContainerElementUIex::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		//this->Select();
		m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMCLICK);
	}
	__super::DoEvent(event);
}
