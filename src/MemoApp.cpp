/***************************************************************
 * Name:      MemoApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Richard (x7536110@outlook.com)
 * Created:   2017-02-12
 * Copyright: Richard ()
 * License:
 **************************************************************/

#include "MemoApp.h"

//(*AppHeaders
#include "MemoMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(MemoApp);

bool MemoApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	MemoDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
