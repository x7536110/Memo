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
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

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
    LogSave();
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
    wxCheckBox* checkbox= new wxCheckBox(this, ID, TextInput->GetValue(), wxPoint(16,23*CheckBoxNum+80), wxSize(260,18), 0, wxDefaultValidator, TextInput->GetValue());
    wxFont checkboxfont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
    checkbox->SetFont(checkboxfont);
    Connect(ID,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MemoDialog::OnCheckBoxClick);

    CheckBox.push_back(checkbox);
    CheckBoxNum++;
    TextInput->Clear();
    TopBar->SetLabel(wxString("按\"添加\"按钮添加一条日志"));
    TopBar->SetForegroundColour(wxColour(64,64,64));

}

//新加checkbox的事件处理
//检测当前checkbox条目的值并做出相应处理
//如果值为真，更改字体式样为下划线式样，灰色，并激活button对象
//如果值为假，更改字体样式为标准式样，黑色，并删除button对象
void MemoDialog::OnCheckBoxClick(wxCommandEvent&  event)
{
    long ID=event.GetId();
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Click!"));

}

//checkbox对应的delete button的事件处理
//销毁对应的checkbox对象
//对其他checkbox对象以及其附属button的位置进行处理
//checkbox对象总数-1
//销毁自身
void MemoDialog::OnButtonDelClick(wxCommandEvent&  event)
{

}

//加载已保存的数据
//读文件，将格式化的数据读入内存
//根据数据内容建立对应控件
void MemoDialog::LogLoad()
{
    wxFile file;
    if(file.Open(wxT("log.txt")))
    {
        file.Close();
        wxFileInputStream input(wxT("log.txt"));
        wxTextInputStream text(input);
        wxString buf;
        wxString num;

        text>>num;
        CheckBoxNum=wxAtoi(num);
        for(int i=0;i<CheckBoxNum;i++)
        {
            wxString label;
            wxString value;
            text>>value>>label;
            if(value=="1")
            {
                long ID=wxNewId();
                wxCheckBox* checkbox= new wxCheckBox(this, ID, label, wxPoint(16,23*i+80), wxSize(260,18), 0, wxDefaultValidator, label);
                wxFont checkboxfont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
                checkbox->SetFont(checkboxfont);
                Connect(ID,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MemoDialog::OnCheckBoxClick);
                checkbox->SetValue(true);
                checkbox->SetForegroundColour(wxColour(100,100,100));
                CheckBox.push_back(checkbox);
            }
            else
            {
                long ID=wxNewId();
                wxCheckBox* checkbox= new wxCheckBox(this, ID, label, wxPoint(16,23*i+80), wxSize(260,18), 0, wxDefaultValidator, label);
                wxFont checkboxfont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
                checkbox->SetFont(checkboxfont);
                Connect(ID,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MemoDialog::OnCheckBoxClick);
                checkbox->SetValue(false);
                checkbox->SetForegroundColour(wxColour(0,0,0));
                CheckBox.push_back(checkbox);
            }

        }
    }
    else
        return;

}

//保存当前数据
//将checkbox的label、value以及checkbox的数目保存至文件并进行base64运算
void MemoDialog::LogSave()
{
    if(CheckBoxNum>0)
    {
        wxString buf;
        buf<<CheckBoxNum<<wxT("\r\n");
        vector<wxCheckBox*>::iterator it;
        for(it=CheckBox.begin();it!=CheckBox.end();it++)
            buf<<(*it)->GetValue()<<wxT(" ")<<(*it)->GetLabel()<<wxT("\r\n");

        wxFile file;
        file.Create(wxT("log.txt"),true);
        file.Write(buf);
        file.Close();
//    checkbox.GetValue()
//    checkbox.GetLabel()
    }
    else
        return;
}

void MemoDialog::OnCheckBox1Click(wxCommandEvent& event)
{
    TopBar->SetForegroundColour(wxColour(255,51,51));
}
