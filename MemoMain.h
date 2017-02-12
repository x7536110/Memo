/***************************************************************
 * Name:      MemoMain.h
 * Purpose:   Defines Application Frame
 * Author:    Richard (x7536110@outlook.com)
 * Created:   2017-02-12
 * Copyright: Richard ()
 * License:
 **************************************************************/

#ifndef MEMOMAIN_H
#define MEMOMAIN_H

//(*Headers(MemoDialog)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#include<wx/checkbox.h>
#include<vector>
#include<queue>

using std::vector;

#define MAX_NOTE 16
class MemoDialog: public wxDialog
{
    public:

        MemoDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~MemoDialog();

    private:

        //(*Handlers(MemoDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButtonAddClick(wxCommandEvent& event);
        void OnCheckBox1Click(wxCommandEvent& event);
        void OnInit(wxInitDialogEvent& event);
        //*)

        //(*Identifiers(MemoDialog)
        static const long ID_TEXTCTRL1;
        static const long ID_BUTTON1;
        static const long ID_STATICTEXT1;
        static const long ID_CHECKBOX1;
        //*)

        //(*Declarations(MemoDialog)
        wxTextCtrl* TextInput;
        wxStaticText* TopBar;
        wxButton* ButtonAdd;
        wxCheckBox* CheckBox1;
        //*)
        void OnCheckBoxClick(wxCommandEvent&  event);
        void OnButtonDelClick(wxCommandEvent& event);
;        void LogLoad();
        void LogSave();
        vector<wxCheckBox*>CheckBox;
        vector<int>Height;
        int CheckBoxNum;
        DECLARE_EVENT_TABLE()
};

#endif // MEMOMAIN_H
