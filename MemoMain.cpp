/***************************************************************
 * Name:      MemoMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Richard (x7536110@outlook.com)
 * Created:   2017-02-12
 * Copyright: Richard ()
 * License:
 **************************************************************/

#include "MemoMain.h"
#include "MemoLogger.h"
#include <wx/msgdlg.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/event.h>
#include <wx/textfile.h>
//STL
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
using std::string;
using std::cin;
using std::cout;
using std::streambuf;
using std::ifstream;
using std::ofstream;

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
    Create(parent, id, _("Memo"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX, _T("id"));
    SetClientSize(wxSize(330,460));
    Move(wxPoint(-1,-1));
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

//添加一项新条目
//微软雅黑12号黑色
//不可包含空格等特殊字符
void MemoDialog::OnButtonAddClick(wxCommandEvent& event)
{
    //输入为空
    if(TextInput->GetValue()==wxEmptyString)
    {
        TopBar->SetLabel(wxString("请输入有效内容"));
        TopBar->SetForegroundColour(wxColour(255,51,51));
        return;
    }
    //达到储存上限
    if(CheckBoxNum==MAX_NOTE)
    {
        TopBar->SetLabel(wxString("日志条目已满，请及时清理"));
        TopBar->SetForegroundColour(wxColour(255,51,51));
        return;
    }
    string temp=TextInput->GetValue().ToStdString();
//    for(int i=0;i<temp.length();i++)
//        if(temp[i]==''||temp[i]=='{'||temp[i]=='}'||temp[i]=='*'||temp[i]=='|'||temp[i]=='\\')
//    {
//        TopBar->SetLabel(wxString("请勿包含{}\"*|#@\\\"等特殊字符"));
//        TopBar->SetForegroundColour(wxColour(255,51,51));
//        return;
//    }



    //添加checkbox
    long ID=wxNewId();
    wxCheckBox* checkbox= new wxCheckBox(
                                         this,
                                         ID,
                                         TextInput->GetValue(),
                                         wxPoint(16,23*CheckBoxNum+80),
                                         wxSize(260,23),
                                         0,
                                         wxDefaultValidator,
                                         TextInput->GetValue());

    wxFont checkboxfont(12,
                        wxFONTFAMILY_SWISS,
                        wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_NORMAL,
                        false,
                        _T("Microsoft YaHei UI"),
                        wxFONTENCODING_DEFAULT);

    checkbox->SetFont(checkboxfont);

    Connect(ID,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MemoDialog::OnCheckBoxClick);
    checkbox->SetValue(false);
    CheckBoxValue[CheckBoxNum]=false;

    //
    MemoButton[CheckBoxNum]=NULL;

    MemoCheckBox[CheckBoxNum]=checkbox;
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
    //触发按键事件后扫描checkbox表，根据表判断触发对象
    wxCheckBox* checkbox=(wxCheckBox*)event.GetEventObject();

    for(int i=0;i<CheckBoxNum;i++)
    {
        //匹配checkbox地址
        if(MemoCheckBox[i]==checkbox)
        {
            //如果checkbox被标记
            if(MemoCheckBox[i]->GetValue()==true)
            {
                //设置字体样式为灰色字体
                MemoCheckBox[i]->SetForegroundColour(wxColour(128,128,128));

                //实例化button对象，用于删除当前checkbox
                long ID=wxNewId();
                wxButton* Button=new wxButton(this,
                                              ID,
                                              _("删除"),
                                              wxPoint((MemoCheckBox[i]->GetPosition()).x+260,
                                                      (MemoCheckBox[i]->GetPosition()).y ),
                                              wxSize(34,20),
                                              0,
                                              wxDefaultValidator,
                                              MemoCheckBox[i]->GetLabel());
                Connect(ID,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MemoDialog::OnButtonDelClick);
                MemoButton[i]=Button;

            }
            //如果是取消标记
            else
            {
                //恢复当前行颜色
                MemoCheckBox[i]->SetForegroundColour(wxColour(0,0,0));
                //销毁已存在的button对象
                MemoButton[i]->Destroy();
                //指针置空
                MemoButton[i]=NULL;

            }
        }
    }

}

//checkbox对应的delete button的事件处理
//销毁对应的checkbox对象
//对其他checkbox对象以及其附属button的位置进行处理
//checkbox对象总数-1
//销毁自身
void MemoDialog::OnButtonDelClick(wxCommandEvent&  event)
{
    //获取事件对象的地址
    wxButton* button=(wxButton*)event.GetEventObject();

    for(int i=0;i<CheckBoxNum;i++)
    {
        //匹配对象
        if(MemoButton[i]==button)
        {
            //处理其他对象的坐标
            for(int j=i+1;j<CheckBoxNum;j++)
            {
                //处理checkbox的坐标
                MemoCheckBox[j]->SetPosition(wxPoint(MemoCheckBox[j]->GetPosition().x,
                                                     MemoCheckBox[j]->GetPosition().y-23));
                //处理对应delbutton的坐标
                if(MemoCheckBox[j]->GetValue()==true)
                    MemoButton[j]->SetPosition(wxPoint(MemoButton[j]->GetPosition().x,
                                                       MemoButton[j]->GetPosition().y-23));
            }

            //销毁编号i对应的对象
            MemoCheckBox[i]->Destroy();
            MemoButton[i]->Destroy();



            //删除数组中对象的指针
            for(int j=i;j<CheckBoxNum-1;j++)
            {
                MemoCheckBox[j]=MemoCheckBox[j+1];
                MemoButton[j]=MemoButton[j+1];

            }
            //num-1

            CheckBoxNum--;
        }
    }
}
//bug::尚不能正确加载中文内容
//加载已保存的数据
//读文件，将格式化的数据读入内存
//根据数据内容建立对应控件
void MemoDialog::LogLoad()
{

    if(!wxFile::Exists(wxT("memo.log")))
        return;

    //加载log文件
    ifstream ifs("memo.log");
    streambuf* old_buffer=cin.rdbuf(ifs.rdbuf());
    cin>>CheckBoxNum;

    //根据log中的内容重建控件列表
    for(int i=0;i<CheckBoxNum;i++)
    {
        string label_s;
        string value_s;
        cin>>value_s;
        getline(cin,label_s);
        wxString label=label_s.c_str();
        wxString value=value_s.c_str();

        if(value=="1")
        {
            long ID=wxNewId();
            wxCheckBox* checkbox= new wxCheckBox(this,
                                                 ID,
                                                 label,
                                                 wxPoint(16,23*i+80),
                                                 wxSize(260,23),
                                                 0,
                                                 wxDefaultValidator,
                                                 label);

            wxFont checkboxfont(12,
                                wxFONTFAMILY_SWISS,
                                wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_NORMAL,
                                false,
                                _T("Microsoft YaHei UI"),
                                wxFONTENCODING_DEFAULT);
            checkbox->SetFont(checkboxfont);

            Connect(ID,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MemoDialog::OnCheckBoxClick);

            checkbox->SetValue(true);
            checkbox->SetForegroundColour(wxColour(100,100,100));
            MemoCheckBox[i]=checkbox;
            CheckBoxValue[i]=true;

            ID=wxNewId();

            wxButton* Button=new wxButton(this,
                                          ID, _("删除"),
                                          wxPoint((checkbox->GetPosition()).x+260,
                                                  (checkbox->GetPosition()).y ),
                                          wxSize(34,20),
                                          0,
                                          wxDefaultValidator,
                                          checkbox->GetLabel());

            Connect(ID,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MemoDialog::OnButtonDelClick);

            MemoButton[i]=Button;

        }
        else
        {
            long ID=wxNewId();
            wxCheckBox* checkbox= new wxCheckBox(this,
                                                 ID,
                                                 label,
                                                 wxPoint(16,23*i+80),
                                                 wxSize(260,23),
                                                 0,
                                                 wxDefaultValidator,
                                                 label);

            wxFont checkboxfont(12,
                                wxFONTFAMILY_SWISS,
                                wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_NORMAL,
                                false,
                                _T("Microsoft YaHei UI"),
                                wxFONTENCODING_DEFAULT);
            checkbox->SetFont(checkboxfont);

            Connect(ID,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MemoDialog::OnCheckBoxClick);

            checkbox->SetValue(false);
            checkbox->SetForegroundColour(wxColour(0,0,0));
            MemoCheckBox[i]=checkbox;
            CheckBoxValue[i]=false;
        }
    }
    cin.rdbuf(old_buffer);
}
//保存当前数据
//将checkbox的label、value以及checkbox的数目保存至文件
void MemoDialog::LogSave()
{
    if(CheckBoxNum>0)
    {
        if(!wxFile::Exists(wxT("memo.log")))
        {
            wxFile file;
            file.Create(wxT("memo.log"),true,wxS_DEFAULT);
        }
        ofstream ofs("memo.log");
        streambuf* old_buffer=cout.rdbuf(ofs.rdbuf());
        cout<<CheckBoxNum<<"\r\n";
        for(int i=0;i<CheckBoxNum;i++)
        {
            cout<<MemoCheckBox[i]->GetValue()
            <<" "<<(const char *)(MemoCheckBox[i]->GetLabel().mb_str())<<"\r\n";
        }
        cout.rdbuf(old_buffer);
    }

}
void MemoDialog::OnCheckBox1Click(wxCommandEvent& event)
{
    TopBar->SetForegroundColour(wxColour(255,51,51));
}
