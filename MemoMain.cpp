/***************************************************************
 * Name:      MemoMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Richard (x7536110@outlook.com)
 * Created:   2017-02-12
 * Copyright: Richard ()
 * License:
 **************************************************************/

#include "MemoMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(MemoDialog)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(MemoDialog)
const long MemoDialog::ID_TEXTCTRL1 = wxNewId();
const long MemoDialog::ID_BUTTON1 = wxNewId();
const long MemoDialog::ID_STATICTEXT1 = wxNewId();
const long MemoDialog::ID_CHECKBOX1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MemoDialog,wxDialog)
    //(*EventTable(MemoDialog)
    //*)
END_EVENT_TABLE()

MemoDialog::MemoDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(MemoDialog)
    Create(parent, id, _("Memo"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(330,460));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK));
    TextInput = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(16,48), wxSize(240,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TextInput->SetMaxLength(15);
    wxFont TextInputFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
    TextInput->SetFont(TextInputFont);
    ButtonAdd = new wxButton(this, ID_BUTTON1, _("添加"), wxPoint(272,48), wxSize(40,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    TopBar = new wxStaticText(this, ID_STATICTEXT1, _("按\"添加\"按钮添加一条日志"), wxPoint(32,14), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    TopBar->SetFocus();
    TopBar->SetForegroundColour(wxColour(64,64,64));
    wxFont TopBarFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
    TopBar->SetFont(TopBarFont);
    CheckBox1 = new wxCheckBox(this, ID_CHECKBOX1, _("Label"), wxPoint(248,16), wxSize(80,14), 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox1->SetValue(false);
    wxFont CheckBox1Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
    CheckBox1->SetFont(CheckBox1Font);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MemoDialog::OnButtonAddClick);
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MemoDialog::OnCheckBox1Click);
    //*)
    CheckBoxNum=0;
    LogLoad();
}

MemoDialog::~MemoDialog()
{
    //(*Destroy(MemoDialog)
    //*)
}

void MemoDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void MemoDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void MemoDialog::OnButtonAddClick(wxCommandEvent& event)
{
    if(TextInput->GetValue()==wxEmptyString)
    {
        //wxString msg = wxbuildinfo(long_f);
        //wxMessageBox(msg,_("EMPTY!"));
        TopBar->SetLabel(wxString("请输入有效内容"));
        TopBar->SetForegroundColour(wxColour(255,51,51));
        return;
    }

    if(CheckBoxNum==MAX_NOTE)
    {
        //wxString msg = wxbuildinfo(long_f);
        //wxMessageBox(msg,_("full!"));
        TopBar->SetLabel(wxString("日志条目已满，请及时清理"));
        TopBar->SetForegroundColour(wxColour(255,51,51));
        return;
    }

    long ID=wxNewId();
    wxCheckBox* checkbox= new wxCheckBox(this, ID, TextInput->GetValue(), wxPoint(16,18*CheckBoxNum+80), wxSize(260,18), 0, wxDefaultValidator, TextInput->GetValue());
    wxFont checkboxfont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
    checkbox->SetFont(checkboxfont);
    CheckBox.push_back(checkbox);
    CheckBoxNum++;
    TextInput->Clear();
    TopBar->SetLabel(wxString("按\"添加\"按钮添加一条日志"));
    TopBar->SetForegroundColour(wxColour(64,64,64));

}

void MemoDialog::LogLoad()
{

}


void MemoDialog::LogSave()
{

}

void MemoDialog::OnCheckBox1Click(wxCommandEvent& event)
{
}
