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
#include <wx/event.h>

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
    ButtonAdd = new wxButton(this, ID_BUTTON1, _("���"), wxPoint(272,48), wxSize(40,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    TopBar = new wxStaticText(this, ID_STATICTEXT1, _("��\"���\"��ť���һ����־"), wxPoint(32,14), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    TopBar->SetFocus();
    TopBar->SetForegroundColour(wxColour(64,64,64));
    wxFont TopBarFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
    TopBar->SetFont(TopBarFont);
    CheckBox1 = new wxCheckBox(this, ID_CHECKBOX1, _("Label"), wxPoint(248,16), wxSize(80,14), 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox1->SetValue(false);
    CheckBox1->Hide();
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

//���һ������Ŀ
//΢���ź�12�ź�ɫ
//���ɰ����ո�������ַ�
void MemoDialog::OnButtonAddClick(wxCommandEvent& event)
{
    if(TextInput->GetValue()==wxEmptyString)
    {
        //wxString msg = wxbuildinfo(long_f);
        //wxMessageBox(msg,_("EMPTY!"));
        TopBar->SetLabel(wxString("��������Ч����"));
        TopBar->SetForegroundColour(wxColour(255,51,51));
        return;
    }

    if(CheckBoxNum==MAX_NOTE)
    {
        //wxString msg = wxbuildinfo(long_f);
        //wxMessageBox(msg,_("full!"));
        TopBar->SetLabel(wxString("��־��Ŀ�������뼰ʱ����"));
        TopBar->SetForegroundColour(wxColour(255,51,51));
        return;
    }

    long ID=wxNewId();
    wxCheckBox* checkbox= new wxCheckBox(this, ID, TextInput->GetValue(), wxPoint(16,23*CheckBoxNum+80), wxSize(260,18), 0, wxDefaultValidator, TextInput->GetValue());
    wxFont checkboxfont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Microsoft YaHei UI"),wxFONTENCODING_DEFAULT);
    checkbox->SetFont(checkboxfont);
    Connect(ID,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MemoDialog::OnCheckBoxClick);
    CheckBoxValue[CheckBoxNum]=false;

    MemoCheckBox.push_back(checkbox);
    CheckBoxNum++;
    TextInput->Clear();
    TopBar->SetLabel(wxString("��\"���\"��ť���һ����־"));
    TopBar->SetForegroundColour(wxColour(64,64,64));

}

//�¼�checkbox���¼�����
//��⵱ǰcheckbox��Ŀ��ֵ��������Ӧ����
//���ֵΪ�棬��������ʽ��Ϊ�»���ʽ������ɫ��������button����
//���ֵΪ�٣�����������ʽΪ��׼ʽ������ɫ����ɾ��button����
void MemoDialog::OnCheckBoxClick(wxCommandEvent&  event)
{
//    long ID=event.GetId();
//    wxString msg = wxbuildinfo(long_f);
//    wxMessageBox(msg, _("Click!"));

    for(int i=0;i<CheckBoxNum;i++)
    {
        if(MemoCheckBox[i]->GetValue()!=CheckBoxValue[i])
        {
            CheckBoxValue[i]=MemoCheckBox[i]->GetValue();
            if(MemoCheckBox[i]->GetValue()==true)
            {
                MemoCheckBox[i]->SetForegroundColour(wxColour(128,128,128));
                long ID=wxNewId();
                wxButton* Button=new wxButton(this, ID, _("ɾ��"), wxPoint((MemoCheckBox[i]->GetPosition()).x+260,(MemoCheckBox[i]->GetPosition()).y ), wxSize(40,22), 0, wxDefaultValidator,MemoCheckBox[i]->GetLabel());
                MemoButton[i]=Button;
            }
            else
            {
                MemoCheckBox[i]->SetForegroundColour(wxColour(0,0,0));
                MemoButton[i]->Destroy();
            }
        }
    }

}

//checkbox��Ӧ��delete button���¼�����
//���ٶ�Ӧ��checkbox����
//������checkbox�����Լ��丽��button��λ�ý��д���
//checkbox��������-1
//��������
void MemoDialog::OnButtonDelClick(wxCommandEvent&  event)
{

}

//�����ѱ��������
//���ļ�������ʽ�������ݶ����ڴ�
//�����������ݽ�����Ӧ�ؼ�
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
                MemoCheckBox.push_back(checkbox);
                CheckBoxValue[i]=true;

                ID=wxNewId();
                wxButton* Button=new wxButton(this, ID, _("ɾ��"), wxPoint((checkbox->GetPosition()).x+260,(checkbox->GetPosition()).y ), wxSize(40,22), 0, wxDefaultValidator,checkbox->GetLabel());
                MemoButton[i]=Button;
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
                MemoCheckBox.push_back(checkbox);
                CheckBoxValue[i]=false;
            }

        }
    }
    else
        return;

}

//���浱ǰ����
//��checkbox��label��value�Լ�checkbox����Ŀ�������ļ�
//����bug���޷���������
void MemoDialog::LogSave()
{
    if(CheckBoxNum>0)
    {
        wxString buf;
        buf<<CheckBoxNum<<wxT("\r\n");
        vector<wxCheckBox*>::iterator it;
        for(it=MemoCheckBox.begin();it!=MemoCheckBox.end();it++)
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
