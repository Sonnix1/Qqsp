#include "qspinputdlg.h"

QspInputDlg::QspInputDlg(QWidget *parent) :
    QWidget(parent)
{
}

QspInputDlg::~QspInputDlg()
{

}

QspInputDlg::QspInputDlg(const QColor &backColor, const QColor &fontColor, const QFont &font, const QString &caption, const QString &text, bool isHtml, const QString &gamePath, QWidget *parent) : QWidget(parent)
{
//    // ----------
//	SetBackgroundColour(backColor);
//	wxSizer *sizerUp = new wxBoxSizer(wxVERTICAL);
//	m_desc = new QSPTextBox(this, ID_INPUT_DESC);
//	m_desc->SetGamePath(gamePath);
//	m_desc->SetIsHtml(isHtml);
//	m_desc->SetBackgroundColour(backColor);
//	m_desc->SetForegroundColour(fontColor);
//	m_desc->SetTextFont(font);
//	m_desc->SetText(text);
//	wxTextCtrl *inputStr = new wxTextCtrl(this, wxID_ANY);
//	inputStr->SetBackgroundColour(backColor);
//	inputStr->SetForegroundColour(fontColor);
//	inputStr->SetFont(font);
//	wxStaticLine* line = new wxStaticLine(this, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
//	sizerUp->Add(m_desc, 1, wxALL | wxGROW, 2);
//	sizerUp->Add(inputStr, 0, wxALL | wxGROW, 2);
//	sizerUp->Add(line, 0, wxALL | wxGROW, 2);
//	// ----------
//	wxSizer *sizerBottom = new wxBoxSizer(wxHORIZONTAL);
//	wxButton *btnOk = new wxButton(this, wxID_OK, _("OK"));
//	wxButton *btnCancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
//	btnOk->SetDefault();
//	btnOk->SetFont(font);
//	btnCancel->SetFont(font);
//	#ifdef __WXMSW__
//		btnOk->SetBackgroundColour(backColor);
//		btnOk->SetForegroundColour(fontColor);
//		btnCancel->SetBackgroundColour(backColor);
//		btnCancel->SetForegroundColour(fontColor);
//	#endif
//	sizerBottom->Add(btnOk, 0, wxALL, 2);
//	sizerBottom->Add(btnCancel, 0, wxALL, 2);
//	// ----------
//	wxSizer *sizerMain = new wxBoxSizer(wxVERTICAL);
//	sizerMain->Add(sizerUp, 1, wxGROW, 0);
//	sizerMain->Add(sizerBottom, 0, wxALIGN_RIGHT, 0);
//	// ----------
//	inputStr->SetValidator(wxGenericValidator(&m_text));
//	static const int minWidth = 420;
//	static const int maxWidth = 550;
//	static const int minHeight = 150;
//	static const int maxHeight = 350;
//	sizerMain->SetMinSize(minWidth, minHeight);
//	SetSizerAndFit(sizerMain);
//	int deltaH = GetClientSize().GetHeight() - m_desc->GetSize().GetHeight();
//	int deltaW = GetClientSize().GetWidth() - m_desc->GetSize().GetWidth();
//	int height = m_desc->GetInternalRepresentation()->GetHeight() + m_desc->GetCharHeight() + deltaH;
//	int width = m_desc->GetInternalRepresentation()->GetWidth() + deltaW;
//	height = wxMin(wxMax(height, minHeight), maxHeight);
//	width = wxMin(wxMax(width, minWidth), maxWidth);
//	SetClientSize(width, height);
//	Center();
//	inputStr->SetFocus();
}
