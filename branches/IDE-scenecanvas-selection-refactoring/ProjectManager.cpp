/** \file
 *  Game Develop
 *  2008-2012 Florian Rival (Florian.Rival@gmail.com)
 */

#include "ProjectManager.h"

//(*InternalHeaders(ProjectManager)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/gallery.h>
#include <wx/ribbon/toolbar.h>
#include <wx/imaglist.h>
#include <wx/busyinfo.h>
#include <fstream>
#include "LogFileManager.h"
#include "GDCore/IDE/Clipboard.h"
#include "MainFrame.h"
#include "gdTreeItemProjectData.h"
#include "GDL/ExternalEvents.h"
#include "GDL/StandardEvent.h"
#include "GDL/CommentEvent.h"
#include "GDL/SourceFile.h"
#include "GDL/Events/CodeCompilationHelpers.h"
#ifdef __WXMSW__
#include <wx/msw/winundef.h>
#endif
#include "CodeEditor.h"
#include "GDCore/IDE/Dialogs/ProjectExtensionsDialog.h"
#include "ExternalEventsEditor.h"
#include "Dialogs/ExternalLayoutEditor.h"
#include "GDCore/IDE/Dialogs/ChooseVariableDialog.h"
#include "EditPropScene.h"
#include "Dialogs/ProjectPropertiesPnl.h"
#include "GDCore/Tools/HelpFileAccess.h"
#include "GDCore/PlatformDefinition/Project.h"
#include "GDCore/PlatformDefinition/Layout.h"
#include "GDCore/PlatformDefinition/ExternalLayout.h"

#include <fstream>

#ifdef __WXMSW__
#include <wx/msw/uxtheme.h>
#endif

using namespace gd;
using namespace GDpriv;

//(*IdInit(ProjectManager)
const long ProjectManager::ID_TREECTRL1 = wxNewId();
const long ProjectManager::idMenuEditScene = wxNewId();
const long ProjectManager::idMenuEditPropScene = wxNewId();
const long ProjectManager::idMenuModVar = wxNewId();
const long ProjectManager::idMenuModNameScene = wxNewId();
const long ProjectManager::idMenuAddScene = wxNewId();
const long ProjectManager::idMenuDelScene = wxNewId();
const long ProjectManager::idMenuCopyScene = wxNewId();
const long ProjectManager::idMenuCutScene = wxNewId();
const long ProjectManager::idMenuPasteScene = wxNewId();
const long ProjectManager::ID_MENUITEM1 = wxNewId();
const long ProjectManager::ID_MENUITEM2 = wxNewId();
const long ProjectManager::ID_MENUITEM3 = wxNewId();
const long ProjectManager::ID_MENUITEM4 = wxNewId();
const long ProjectManager::ID_MENUITEM5 = wxNewId();
const long ProjectManager::ID_MENUITEM6 = wxNewId();
const long ProjectManager::ID_MENUITEM7 = wxNewId();
const long ProjectManager::ID_MENUITEM13 = wxNewId();
const long ProjectManager::ID_MENUITEM8 = wxNewId();
const long ProjectManager::ID_MENUITEM9 = wxNewId();
const long ProjectManager::ID_MENUITEM10 = wxNewId();
const long ProjectManager::ID_MENUITEM11 = wxNewId();
const long ProjectManager::ID_MENUITEM12 = wxNewId();
const long ProjectManager::ID_MENUITEM18 = wxNewId();
const long ProjectManager::ID_MENUITEM14 = wxNewId();
const long ProjectManager::ID_MENUITEM15 = wxNewId();
const long ProjectManager::ID_MENUITEM16 = wxNewId();
const long ProjectManager::ID_MENUITEM19 = wxNewId();
const long ProjectManager::ID_MENUITEM17 = wxNewId();
const long ProjectManager::ID_MENUITEM20 = wxNewId();
const long ProjectManager::ID_MENUITEM21 = wxNewId();
const long ProjectManager::ID_MENUITEM22 = wxNewId();
const long ProjectManager::ID_MENUITEM23 = wxNewId();
const long ProjectManager::ID_MENUITEM24 = wxNewId();
const long ProjectManager::ID_MENUITEM25 = wxNewId();
const long ProjectManager::ID_MENUITEM26 = wxNewId();
const long ProjectManager::ID_MENUITEM27 = wxNewId();
//*)
const long ProjectManager::idRibbonNew = wxNewId();
const long ProjectManager::idRibbonOpen = wxNewId();
const long ProjectManager::idRibbonSave = wxNewId();
const long ProjectManager::idRibbonSaveAll = wxNewId();
const long ProjectManager::idRibbonCompil = wxNewId();
const long ProjectManager::idRibbonClose = wxNewId();
const long ProjectManager::idRibbonExtensions = wxNewId();
const long ProjectManager::idRibbonAddScene = wxNewId();
const long ProjectManager::idRibbonEditImages = wxNewId();
const long ProjectManager::idRibbonEditSelected = wxNewId();
const long ProjectManager::idRibbonAddExternalEvents = wxNewId();
const long ProjectManager::idRibbonAddExternalLayout = wxNewId();
const long ProjectManager::idRibbonStartPage = wxNewId();
const long ProjectManager::idRibbonCppTools = wxNewId();
const long ProjectManager::idRibbonImporter = wxNewId();
const long ProjectManager::idRibbonEncoder = wxNewId();
const long ProjectManager::idRibbonProjectsManager = wxNewId();
const long ProjectManager::idRibbonHelp = wxNewId();

BEGIN_EVENT_TABLE(ProjectManager,wxPanel)
	//(*EventTable(ProjectManager)
	//*)
END_EVENT_TABLE()

ProjectManager::ProjectManager(wxWindow* parent, MainFrame & mainEditor_) :
mainEditor(mainEditor_)
{
	//(*Initialize(ProjectManager)
	wxMenuItem* deleteSceneMenuItem;
	wxMenuItem* editSceneNameMenuItem;
	wxMenuItem* editSceneMenuItem;
	wxFlexGridSizer* FlexGridSizer1;
	wxMenuItem* addSceneMenuItem;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	projectsTree = new wxTreeCtrl(this, ID_TREECTRL1, wxDefaultPosition, wxSize(209,197), wxTR_EDIT_LABELS|wxTR_HIDE_ROOT|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRL1"));
	FlexGridSizer1->Add(projectsTree, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	SetSizer(FlexGridSizer1);
	editSceneMenuItem = new wxMenuItem((&sceneContextMenu), idMenuEditScene, _("Edit this scene"), wxEmptyString, wxITEM_NORMAL);
	editSceneMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/editicon.png"))));
	sceneContextMenu.Append(editSceneMenuItem);
	editScenePropMenuItem = new wxMenuItem((&sceneContextMenu), idMenuEditPropScene, _("Edit the properties"), wxEmptyString, wxITEM_NORMAL);
	editScenePropMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/editpropicon.png"))));
	sceneContextMenu.Append(editScenePropMenuItem);
	modVarSceneMenuI = new wxMenuItem((&sceneContextMenu), idMenuModVar, _("Modify inital variables"), wxEmptyString, wxITEM_NORMAL);
	modVarSceneMenuI->SetBitmap(wxBitmap(wxImage(_T("res/var.png"))));
	sceneContextMenu.Append(modVarSceneMenuI);
	editSceneNameMenuItem = new wxMenuItem((&sceneContextMenu), idMenuModNameScene, _("Change the name"), wxEmptyString, wxITEM_NORMAL);
	editSceneNameMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/editnom.png"))));
	sceneContextMenu.Append(editSceneNameMenuItem);
	sceneContextMenu.AppendSeparator();
	addSceneMenuItem = new wxMenuItem((&sceneContextMenu), idMenuAddScene, _("Add a scene"), wxEmptyString, wxITEM_NORMAL);
	addSceneMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/addicon.png"))));
	sceneContextMenu.Append(addSceneMenuItem);
	deleteSceneMenuItem = new wxMenuItem((&sceneContextMenu), idMenuDelScene, _("Delete the scene"), wxEmptyString, wxITEM_NORMAL);
	deleteSceneMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/deleteicon.png"))));
	sceneContextMenu.Append(deleteSceneMenuItem);
	sceneContextMenu.AppendSeparator();
	copySceneMenuItem = new wxMenuItem((&sceneContextMenu), idMenuCopyScene, _("Copy the scene"), wxEmptyString, wxITEM_NORMAL);
	copySceneMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/copyicon.png"))));
	sceneContextMenu.Append(copySceneMenuItem);
	cutSceneMenuItem = new wxMenuItem((&sceneContextMenu), idMenuCutScene, _("Cut the scene"), wxEmptyString, wxITEM_NORMAL);
	cutSceneMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/cuticon.png"))));
	sceneContextMenu.Append(cutSceneMenuItem);
	pasteSceneMenuItem = new wxMenuItem((&sceneContextMenu), idMenuPasteScene, _("Paste the scne"), wxEmptyString, wxITEM_NORMAL);
	pasteSceneMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/pasteicon.png"))));
	sceneContextMenu.Append(pasteSceneMenuItem);
	MenuItem1 = new wxMenuItem((&scenesContextMenu), ID_MENUITEM1, _("Add a scene"), wxEmptyString, wxITEM_NORMAL);
	MenuItem1->SetBitmap(wxBitmap(wxImage(_T("res/addicon.png"))));
	scenesContextMenu.Append(MenuItem1);
	editPropGameMenuItem = new wxMenuItem((&gameContextMenu), ID_MENUITEM2, _("Edit the property of the game"), wxEmptyString, wxITEM_NORMAL);
	editPropGameMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/editpropicon.png"))));
	gameContextMenu.Append(editPropGameMenuItem);
	editGblVarMenuItem = new wxMenuItem((&gameContextMenu), ID_MENUITEM3, _("Modify global variables"), wxEmptyString, wxITEM_NORMAL);
	editGblVarMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/var.png"))));
	gameContextMenu.Append(editGblVarMenuItem);
	editNameGameMenuItem = new wxMenuItem((&gameContextMenu), ID_MENUITEM4, _("Change the name"), wxEmptyString, wxITEM_NORMAL);
	editNameGameMenuItem->SetBitmap(wxBitmap(wxImage(_T("res/editnom.png"))));
	gameContextMenu.Append(editNameGameMenuItem);
	gameContextMenu.AppendSeparator();
	closeGameBt = new wxMenuItem((&gameContextMenu), ID_MENUITEM5, _("Close this project"), wxEmptyString, wxITEM_NORMAL);
	gameContextMenu.Append(closeGameBt);
	MenuItem2 = new wxMenuItem((&emptyExternalEventsContextMenu), ID_MENUITEM6, _("Add external events"), wxEmptyString, wxITEM_NORMAL);
	MenuItem2->SetBitmap(wxBitmap(wxImage(_T("res/eventsadd16.png"))));
	emptyExternalEventsContextMenu.Append(MenuItem2);
	MenuItem3 = new wxMenuItem((&externalEventsContextMenu), ID_MENUITEM7, _("Edit"), wxEmptyString, wxITEM_NORMAL);
	MenuItem3->SetBitmap(wxBitmap(wxImage(_T("res/eventsedit16.png"))));
	externalEventsContextMenu.Append(MenuItem3);
	MenuItem9 = new wxMenuItem((&externalEventsContextMenu), ID_MENUITEM13, _("Rename"), wxEmptyString, wxITEM_NORMAL);
	MenuItem9->SetBitmap(wxBitmap(wxImage(_T("res/editnom.png"))));
	externalEventsContextMenu.Append(MenuItem9);
	externalEventsContextMenu.AppendSeparator();
	MenuItem4 = new wxMenuItem((&externalEventsContextMenu), ID_MENUITEM8, _("Add external events"), wxEmptyString, wxITEM_NORMAL);
	MenuItem4->SetBitmap(wxBitmap(wxImage(_T("res/eventsadd16.png"))));
	externalEventsContextMenu.Append(MenuItem4);
	MenuItem5 = new wxMenuItem((&externalEventsContextMenu), ID_MENUITEM9, _("Delete"), wxEmptyString, wxITEM_NORMAL);
	MenuItem5->SetBitmap(wxBitmap(wxImage(_T("res/deleteicon.png"))));
	externalEventsContextMenu.Append(MenuItem5);
	externalEventsContextMenu.AppendSeparator();
	MenuItem6 = new wxMenuItem((&externalEventsContextMenu), ID_MENUITEM10, _("Copy"), wxEmptyString, wxITEM_NORMAL);
	MenuItem6->SetBitmap(wxBitmap(wxImage(_T("res/copyicon.png"))));
	externalEventsContextMenu.Append(MenuItem6);
	MenuItem7 = new wxMenuItem((&externalEventsContextMenu), ID_MENUITEM11, _("Cut"), wxEmptyString, wxITEM_NORMAL);
	MenuItem7->SetBitmap(wxBitmap(wxImage(_T("res/cuticon.png"))));
	externalEventsContextMenu.Append(MenuItem7);
	MenuItem8 = new wxMenuItem((&externalEventsContextMenu), ID_MENUITEM12, _("Paste"), wxEmptyString, wxITEM_NORMAL);
	MenuItem8->SetBitmap(wxBitmap(wxImage(_T("res/pasteicon.png"))));
	externalEventsContextMenu.Append(MenuItem8);
	MenuItem14 = new wxMenuItem((&sourceFilesContextMenu), ID_MENUITEM18, _("Create a new C++ file"), wxEmptyString, wxITEM_NORMAL);
	sourceFilesContextMenu.Append(MenuItem14);
	MenuItem10 = new wxMenuItem((&sourceFilesContextMenu), ID_MENUITEM14, _("Add an already existing C++ file"), wxEmptyString, wxITEM_NORMAL);
	MenuItem10->SetBitmap(wxBitmap(wxImage(_T("res/addicon.png"))));
	sourceFilesContextMenu.Append(MenuItem10);
	MenuItem11 = new wxMenuItem((&sourceFileContextMenu), ID_MENUITEM15, _("Edit"), wxEmptyString, wxITEM_NORMAL);
	MenuItem11->SetBitmap(wxBitmap(wxImage(_T("res/editicon.png"))));
	sourceFileContextMenu.Append(MenuItem11);
	sourceFileContextMenu.AppendSeparator();
	MenuItem12 = new wxMenuItem((&sourceFileContextMenu), ID_MENUITEM16, _("Delete"), wxEmptyString, wxITEM_NORMAL);
	MenuItem12->SetBitmap(wxBitmap(wxImage(_T("res/deleteicon.png"))));
	sourceFileContextMenu.Append(MenuItem12);
	sourceFileContextMenu.AppendSeparator();
	MenuItem15 = new wxMenuItem((&sourceFileContextMenu), ID_MENUITEM19, _("Create a new C++ file"), wxEmptyString, wxITEM_NORMAL);
	sourceFileContextMenu.Append(MenuItem15);
	MenuItem13 = new wxMenuItem((&sourceFileContextMenu), ID_MENUITEM17, _("Add an already existing C++ file"), wxEmptyString, wxITEM_NORMAL);
	MenuItem13->SetBitmap(wxBitmap(wxImage(_T("res/addicon.png"))));
	sourceFileContextMenu.Append(MenuItem13);
	MenuItem16 = new wxMenuItem((&emptyExternalLayoutsContextMenu), ID_MENUITEM20, _("Add an external layout"), wxEmptyString, wxITEM_NORMAL);
	MenuItem16->SetBitmap(wxBitmap(wxImage(_T("res/eventsadd16.png"))));
	emptyExternalLayoutsContextMenu.Append(MenuItem16);
	MenuItem17 = new wxMenuItem((&externalLayoutContextMenu), ID_MENUITEM21, _("Edit"), wxEmptyString, wxITEM_NORMAL);
	MenuItem17->SetBitmap(wxBitmap(wxImage(_T("res/eventsedit16.png"))));
	externalLayoutContextMenu.Append(MenuItem17);
	MenuItem18 = new wxMenuItem((&externalLayoutContextMenu), ID_MENUITEM22, _("Rename"), wxEmptyString, wxITEM_NORMAL);
	MenuItem18->SetBitmap(wxBitmap(wxImage(_T("res/editnom.png"))));
	externalLayoutContextMenu.Append(MenuItem18);
	externalLayoutContextMenu.AppendSeparator();
	MenuItem19 = new wxMenuItem((&externalLayoutContextMenu), ID_MENUITEM23, _("Add an external layout"), wxEmptyString, wxITEM_NORMAL);
	MenuItem19->SetBitmap(wxBitmap(wxImage(_T("res/eventsadd16.png"))));
	externalLayoutContextMenu.Append(MenuItem19);
	MenuItem20 = new wxMenuItem((&externalLayoutContextMenu), ID_MENUITEM24, _("Delete"), wxEmptyString, wxITEM_NORMAL);
	MenuItem20->SetBitmap(wxBitmap(wxImage(_T("res/deleteicon.png"))));
	externalLayoutContextMenu.Append(MenuItem20);
	externalLayoutContextMenu.AppendSeparator();
	MenuItem21 = new wxMenuItem((&externalLayoutContextMenu), ID_MENUITEM25, _("Copy"), wxEmptyString, wxITEM_NORMAL);
	MenuItem21->SetBitmap(wxBitmap(wxImage(_T("res/copyicon.png"))));
	externalLayoutContextMenu.Append(MenuItem21);
	MenuItem22 = new wxMenuItem((&externalLayoutContextMenu), ID_MENUITEM26, _("Cut"), wxEmptyString, wxITEM_NORMAL);
	MenuItem22->SetBitmap(wxBitmap(wxImage(_T("res/cuticon.png"))));
	externalLayoutContextMenu.Append(MenuItem22);
	MenuItem23 = new wxMenuItem((&externalLayoutContextMenu), ID_MENUITEM27, _("Paste"), wxEmptyString, wxITEM_NORMAL);
	MenuItem23->SetBitmap(wxBitmap(wxImage(_T("res/pasteicon.png"))));
	externalLayoutContextMenu.Append(MenuItem23);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT,(wxObjectEventFunction)&ProjectManager::OnprojectsTreeBeginLabelEdit);
	Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_END_LABEL_EDIT,(wxObjectEventFunction)&ProjectManager::OnprojectsTreeEndLabelEdit);
	Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_ITEM_ACTIVATED,(wxObjectEventFunction)&ProjectManager::OnprojectsTreeItemActivated);
	Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&ProjectManager::OnprojectsTreeItemRightClick);
	Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&ProjectManager::OnprojectsTreeSelectionChanged);
	Connect(idMenuEditScene,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OneditSceneMenuItemSelected);
	Connect(idMenuEditPropScene,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OneditScenePropMenuItemSelected);
	Connect(idMenuModVar,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnmodVarSceneMenuISelected);
	Connect(idMenuModNameScene,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OneditSceneNameMenuItemSelected);
	Connect(idMenuAddScene,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnaddSceneMenuItemSelected);
	Connect(idMenuDelScene,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OndeleteSceneMenuItemSelected);
	Connect(idMenuCopyScene,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OncopySceneMenuItemSelected);
	Connect(idMenuCutScene,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OncutSceneMenuItemSelected);
	Connect(idMenuPasteScene,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnpasteSceneMenuItemSelected);
	Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnaddSceneMenuItemSelected);
	Connect(ID_MENUITEM2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OneditPropGameMenuItemSelected);
	Connect(ID_MENUITEM3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OneditGblVarMenuItemSelected);
	Connect(ID_MENUITEM4,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OneditNameGameMenuItemSelected);
	Connect(ID_MENUITEM5,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OncloseGameBtSelected);
	Connect(ID_MENUITEM6,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnAddExternalEventsSelected);
	Connect(ID_MENUITEM7,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnEditExternalEventsSelected);
	Connect(ID_MENUITEM13,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnRenameExternalEventsSelected);
	Connect(ID_MENUITEM8,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnAddExternalEventsSelected);
	Connect(ID_MENUITEM9,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnDeleteExternalEventsSelected);
	Connect(ID_MENUITEM10,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnCopyExternalEventsSelected);
	Connect(ID_MENUITEM11,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnCutExternalEventsSelected);
	Connect(ID_MENUITEM12,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnPasteExternalEventsSelected);
	Connect(ID_MENUITEM18,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnCreateNewCppFileSelected);
	Connect(ID_MENUITEM14,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnAddCppSourceFileSelected);
	Connect(ID_MENUITEM15,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnEditSourceFileSelected);
	Connect(ID_MENUITEM16,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnDeleteSourceFileSelected);
	Connect(ID_MENUITEM19,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnCreateNewCppFileSelected);
	Connect(ID_MENUITEM17,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnAddCppSourceFileSelected);
	Connect(ID_MENUITEM20,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnAddExternalLayoutSelected);
	Connect(ID_MENUITEM21,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnEditExternalLayoutSelected);
	Connect(ID_MENUITEM22,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnRenameExternalLayoutSelected);
	Connect(ID_MENUITEM23,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnAddExternalLayoutSelected);
	Connect(ID_MENUITEM24,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnDeleteExternalLayoutSelected);
	Connect(ID_MENUITEM25,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnCopyExternalLayoutSelected);
	Connect(ID_MENUITEM26,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnCutExternalLayoutSelected);
	Connect(ID_MENUITEM27,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ProjectManager::OnPasteExternalLayoutSelected);
	//*)

    wxImageList * imageList = new wxImageList(16,16);
    imageList->Add(wxBitmap("res/window.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/sceneeditor.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/imageicon.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/extensiononly16.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/events16.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/source_cpp16.png", wxBITMAP_TYPE_ANY));
    imageList->Add(wxBitmap("res/sceneeditor.png", wxBITMAP_TYPE_ANY));
    projectsTree->SetImageList(imageList);

    #if defined(__WXMSW__) //Offers nice look to wxTreeCtrl
    wxUxThemeEngine* theme =  wxUxThemeEngine::GetIfActive();
    if(theme) theme->SetWindowTheme((HWND) projectsTree->GetHWND(), L"EXPLORER", NULL);
    #endif

    Refresh();
}

ProjectManager::~ProjectManager()
{
	//(*Destroy(ProjectManager)
	//*)
}

/**
 * Static method for creating ribbon page for this kind of editor
 */
void ProjectManager::CreateRibbonPage(wxRibbonPage * page)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    bool hideLabels = false;
    pConfig->Read( _T( "/Skin/HideLabels" ), &hideLabels );

    {
        wxRibbonPanel *ribbonPanel = new wxRibbonPanel(page, wxID_ANY, _("Projects"), wxBitmap("res/openicon.png", wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE);
        wxRibbonButtonBar *ribbonBar = new wxRibbonButtonBar(ribbonPanel, wxID_ANY);
        ribbonBar->AddButton(idRibbonNew, !hideLabels ? _("New") : "", wxBitmap("res/newicon24.png", wxBITMAP_TYPE_ANY));
        ribbonBar->AddHybridButton(idRibbonOpen, !hideLabels ? _("Open") : "", wxBitmap("res/openicon24.png", wxBITMAP_TYPE_ANY));
    }
    {
        wxRibbonPanel *file2Panel = new wxRibbonPanel(page, wxID_ANY, _("Current project"), wxBitmap("res/saveicon.png", wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);
        wxRibbonButtonBar *file2_bar = new wxRibbonButtonBar(file2Panel, wxID_ANY);
        file2_bar->AddHybridButton(idRibbonSave, !hideLabels ? _("Save") : " ", wxBitmap("res/saveicon24.png", wxBITMAP_TYPE_ANY));
        file2_bar->AddButton(idRibbonSaveAll, !hideLabels ? _("Save all") : " ", wxBitmap("res/save_all24.png", wxBITMAP_TYPE_ANY));
        file2_bar->AddButton(idRibbonClose, !hideLabels ? _("Close") : "", wxBitmap("res/close24.png", wxBITMAP_TYPE_ANY));
        file2_bar->AddButton(idRibbonCompil, !hideLabels ? _("Compilation") : "", wxBitmap("res/compilicon24.png", wxBITMAP_TYPE_ANY));
    }
    {
        wxRibbonPanel *ribbonPanel = new wxRibbonPanel(page, wxID_ANY, _("Basic Operations"), wxBitmap("res/copy24.png", wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE);
        wxRibbonButtonBar *ribbonBar = new wxRibbonButtonBar(ribbonPanel, wxID_ANY);
        ribbonBar->AddButton(idRibbonEditImages, !hideLabels ? _("Images") : "", wxBitmap("res/imageicon24.png", wxBITMAP_TYPE_ANY));
        ribbonBar->AddButton(idRibbonAddScene, !hideLabels ? _("Add a scene") : "", wxBitmap("res/sceneadd24.png", wxBITMAP_TYPE_ANY));
        ribbonBar->AddButton(idRibbonAddExternalEvents, !hideLabels ? _("Add external events") : "", wxBitmap("res/eventsadd24.png", wxBITMAP_TYPE_ANY));
        ribbonBar->AddButton(idRibbonAddExternalLayout, !hideLabels ? _("Add an external layout") : "", wxBitmap("res/sceneadd24.png", wxBITMAP_TYPE_ANY));
        ribbonBar->AddButton(idRibbonExtensions, !hideLabels ? _("Extensions") : "", wxBitmap("res/extension24.png", wxBITMAP_TYPE_ANY));
    }
    {
        wxRibbonPanel *affichagePanel = new wxRibbonPanel(page, wxID_ANY, _("View"), wxBitmap("res/imageicon.png", wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE);
        wxRibbonButtonBar *affichage_bar = new wxRibbonButtonBar(affichagePanel, wxID_ANY);
        affichage_bar->AddButton(idRibbonProjectsManager, !hideLabels ? _("Projects") : "", wxBitmap("res/projectManager24.png", wxBITMAP_TYPE_ANY));
        affichage_bar->AddButton(idRibbonStartPage, !hideLabels ? _("Start page") : "", wxBitmap("res/startPage24.png", wxBITMAP_TYPE_ANY));
        affichage_bar->AddButton(idRibbonCppTools, !hideLabels ? _("C++ Tools") : "", wxBitmap("res/source_cpp24.png", wxBITMAP_TYPE_ANY));

        wxRibbonPanel *toolsPanel = new wxRibbonPanel(page, wxID_ANY, _("Tools"), wxBitmap("res/tools24.png", wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE);
        wxRibbonButtonBar *tools_bar = new wxRibbonButtonBar(toolsPanel, wxID_ANY);
        tools_bar->AddButton(idRibbonEncoder, !hideLabels ? _("Converter") : "", wxBitmap("res/musicicon24.png", wxBITMAP_TYPE_ANY));
        tools_bar->AddDropdownButton(idRibbonImporter, !hideLabels ? _("Image converter") : "", wxBitmap("res/strip24.png", wxBITMAP_TYPE_ANY));

    }
    {
        wxRibbonPanel *ribbonPanel = new wxRibbonPanel(page, wxID_ANY, _("Help"), wxBitmap("res/helpicon24.png", wxBITMAP_TYPE_ANY), wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE);
        wxRibbonButtonBar *ribbonBar = new wxRibbonButtonBar(ribbonPanel, wxID_ANY);
        ribbonBar->AddButton(idRibbonHelp, !hideLabels ? _("Help") : "", wxBitmap("res/helpicon24.png", wxBITMAP_TYPE_ANY));
    }

}

void ProjectManager::ConnectEvents()
{
    mainEditor.Connect( idRibbonNew, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnMenuNewSelected, NULL, &mainEditor );
    mainEditor.Connect( idRibbonOpen, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnMenuOpenSelected, NULL, &mainEditor );
    mainEditor.Connect( idRibbonSaveAll, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnRibbonSaveAllClicked, NULL, &mainEditor );
    mainEditor.Connect( idRibbonOpen, wxEVT_COMMAND_RIBBONBUTTON_DROPDOWN_CLICKED, ( wxObjectEventFunction )&MainFrame::OnRibbonOpenDropDownClicked, NULL, &mainEditor );
    mainEditor.Connect( idRibbonSave, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnMenuSaveSelected, NULL, &mainEditor );
    mainEditor.Connect( idRibbonSave, wxEVT_COMMAND_RIBBONBUTTON_DROPDOWN_CLICKED, ( wxObjectEventFunction )&MainFrame::OnRibbonSaveDropDownClicked, NULL, &mainEditor );
    mainEditor.Connect( idRibbonCompil, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnMenuCompilationSelected, NULL, &mainEditor );
    mainEditor.Connect( idRibbonClose, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&ProjectManager::OnRibbonCloseSelected, NULL, this );
    mainEditor.Connect( idRibbonExtensions, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&ProjectManager::OnRibbonExtensionsSelected, NULL, this );
    mainEditor.Connect( idRibbonEditImages, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&ProjectManager::OnRibbonEditImagesSelected, NULL, this );
    mainEditor.Connect( idRibbonAddScene, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&ProjectManager::OnRibbonAddSceneSelected, NULL, this );
    mainEditor.Connect( idRibbonAddExternalEvents, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&ProjectManager::OnRibbonAddExternalEventsSelected, NULL, this );
    mainEditor.Connect( idRibbonAddExternalLayout, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&ProjectManager::OnRibbonAddExternalLayoutSelected, NULL, this );
    mainEditor.Connect( idRibbonStartPage, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnRibbonStartPageClicked, NULL, &mainEditor );
    mainEditor.Connect( idRibbonCppTools, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnRibbonCppToolsClicked, NULL, &mainEditor );
    mainEditor.Connect( idRibbonProjectsManager, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnProjectsManagerClicked, NULL, &mainEditor );
    mainEditor.Connect( idRibbonEncoder, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&MainFrame::OnMenuItem23Selected, NULL, &mainEditor );
    mainEditor.Connect( idRibbonImporter, wxEVT_COMMAND_RIBBONBUTTON_DROPDOWN_CLICKED, ( wxObjectEventFunction )&MainFrame::OnRibbonDecomposerDropDownClicked, NULL, &mainEditor );
    mainEditor.Connect( idRibbonHelp, wxEVT_COMMAND_RIBBONBUTTON_CLICKED, ( wxObjectEventFunction )&ProjectManager::OnRibbonHelpSelected, NULL, &mainEditor );

}

void ProjectManager::Refresh()
{
    projectsTree->DeleteAllItems();
    projectsTree->AddRoot(_("Projects"));
    for (unsigned int i = 0;i<mainEditor.games.size();++i)
    {
        wxString name = mainEditor.games[i]->GetName() == "" ?
                        _("(No name)") :
                        mainEditor.games[i]->GetName();

        //Adding game's root
        gdTreeItemProjectData * gameItemData = new gdTreeItemProjectData("Root", "", mainEditor.games[i].get());
        wxTreeItemId projectItem = projectsTree->AppendItem(projectsTree->GetRootItem(), name, 0, 0, gameItemData);
        if ( mainEditor.gameCurrentlyEdited == i) projectsTree->SetItemBold(projectItem, true);

        //Images
        gdTreeItemProjectData * imagesItemData = new gdTreeItemProjectData("Images", "", mainEditor.games[i].get());
        projectsTree->AppendItem(projectItem, _("Images"), 2 ,2, imagesItemData);

        //Scenes
        gdTreeItemProjectData * scenesItemData = new gdTreeItemProjectData("Scenes", "", mainEditor.games[i].get());
        wxTreeItemId scenesItem = projectsTree->AppendItem(projectItem, _("Scenes"), 1 ,1, scenesItemData);
        for (unsigned int j = 0;j<mainEditor.games[i]->GetLayoutCount();++j)
        {
            gdTreeItemProjectData * sceneItemData = new gdTreeItemProjectData("Scene", mainEditor.games[i]->GetLayout(j).GetName(), mainEditor.games[i].get());
            projectsTree->AppendItem(scenesItem, mainEditor.games[i]->GetLayout(j).GetName(), 1 ,1, sceneItemData);
        }

        //Evenements externes
        gdTreeItemProjectData * eventsItemData = new gdTreeItemProjectData("ExternalEventsRoot", "", mainEditor.games[i].get());
        wxTreeItemId eventsItem = projectsTree->AppendItem(projectItem, _("External events"), 4 ,4, eventsItemData);
        for (unsigned int j = 0;j<mainEditor.games[i]->GetExternalEventsCount();++j)
        {
            gdTreeItemProjectData * eventsItemData = new gdTreeItemProjectData("ExternalEvents", mainEditor.games[i]->GetExternalEvents(j).GetName(), mainEditor.games[i].get());
            projectsTree->AppendItem(eventsItem, mainEditor.games[i]->GetExternalEvents(j).GetName(), 4 ,4, eventsItemData);
        }

        gdTreeItemProjectData * externalLayoutsItemData = new gdTreeItemProjectData("ExternalLayoutsRoot", "", mainEditor.games[i].get());
        wxTreeItemId externalayoutsItem = projectsTree->AppendItem(projectItem, _("External layouts"), 6 , 6, externalLayoutsItemData);
        for (unsigned int j = 0;j<mainEditor.games[i]->GetExternalLayoutsCount();++j)
        {
            gdTreeItemProjectData * externalLayoutsItemData = new gdTreeItemProjectData("ExternalLayout", mainEditor.games[i]->GetExternalLayout(j).GetName(), mainEditor.games[i].get());
            projectsTree->AppendItem(externalayoutsItem, mainEditor.games[i]->GetExternalLayout(j).GetName(), 6 , 6, externalLayoutsItemData);
        }

        boost::shared_ptr<RuntimeGame> game = boost::dynamic_pointer_cast<RuntimeGame>(mainEditor.games[i]);
        if ( game !=  boost::shared_ptr<RuntimeGame>() && game->useExternalSourceFiles )
        {
            gdTreeItemProjectData * sourceFilesItemData = new gdTreeItemProjectData("SourceFiles", "", mainEditor.games[i].get());
            wxTreeItemId sourceFilesItem = projectsTree->AppendItem(projectItem, _("C++ source files"), 5 ,5, sourceFilesItemData);
            for (unsigned int j = 0;j<game->externalSourceFiles.size();++j)
            {
                if ( game->externalSourceFiles[j]->IsGDManaged() )
                    continue;

                gdTreeItemProjectData * sourceFileItem = new gdTreeItemProjectData("SourceFile", game->externalSourceFiles[j]->GetFileName(), game.get());
                projectsTree->AppendItem(sourceFilesItem, game->externalSourceFiles[j]->GetFileName(), 5 ,5, sourceFileItem);
            }
        }

        //Extensions
        gdTreeItemProjectData * extensionsItemData = new gdTreeItemProjectData("Extensions", "", mainEditor.games[i].get());
        projectsTree->AppendItem(projectItem, _("Extensions") + " (" + ToString(mainEditor.games[i]->GetUsedPlatformExtensions().size()) + ")", 3 ,3, extensionsItemData);
    }

    projectsTree->ExpandAll();

    if ( !mainEditor.games.empty() )
        projectsTree->SetToolTip(_("Double click to set the project as the current project.\nDouble click on an item to edit it, or use right\nclick to display more options."));
    else
        projectsTree->SetToolTip(_("Create or open a project using the ribbon."));
}

/**
 * Complete the pointers with the game and the datas corresponding to the selected item.
 * Return false if fail, in which case pointers are invalid.
 */
bool ProjectManager::GetGameOfSelectedItem(RuntimeGame *& game, gdTreeItemProjectData *& data)
{
    if ( !selectedItem.IsOk() ) return false;

    data = dynamic_cast<gdTreeItemProjectData*>(projectsTree->GetItemData(selectedItem));
    if ( data == NULL )
        return false;

    game = dynamic_cast<RuntimeGame*>(data->GetGamePointer());
    if ( game == NULL )
        return false;

    return true;
}

/**
 * Double click on an item
 */
void ProjectManager::OnprojectsTreeItemActivated(wxTreeEvent& event)
{
    selectedItem = event.GetItem();

    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    string prefix = "";
    if ( mainEditor.games.size() > 1 )
    {
        prefix = "["+game->GetName()+"] ";
        if ( game->GetName().length() > gameMaxCharDisplayedInEditor )
            prefix = "["+game->GetName().substr(0, gameMaxCharDisplayedInEditor-3)+"...] ";
    }

    if ( data->GetString() == "Root")
    {
        //Set all projects font to a normal font.
        wxTreeItemIdValue cookie;
        wxTreeItemId child = projectsTree->GetFirstChild(projectsTree->GetRootItem(), cookie);
        while(child.IsOk())
        {
            projectsTree->SetItemBold(child, false);

            child = projectsTree->GetNextChild(projectsTree->GetRootItem(), cookie);
        }

        //Activate the project double clicked
        for(unsigned int i = 0;i<mainEditor.games.size();++i)
        {
            if (mainEditor.games[i].get() == game)
            {
                projectsTree->SetItemBold(selectedItem, true);
                mainEditor.SetCurrentGame(i, /*refreshProjectManager=*/false);
            }
        }
    }
    else if ( data->GetString() == "Images")
    {
        EditResourcesOfProject(game);
    }
    else if ( data->GetString() == "Scene")
    {
        wxCommandEvent unusedEvent;
        OneditSceneMenuItemSelected(unusedEvent);
    }
    else if ( data->GetString() == "ExternalEvents")
    {
        wxCommandEvent unusedEvent;
        OnEditExternalEventsSelected(unusedEvent);
    }
    else if ( data->GetString() == "ExternalLayout")
    {
        wxCommandEvent unusedEvent;
        OnEditExternalLayoutSelected(unusedEvent);
    }
    else if ( data->GetString() == "Extensions")
    {
        EditExtensionsOfGame(*game);

        projectsTree->SetItemText(selectedItem, _("Extensions") + " (" + ToString(game->GetUsedPlatformExtensions().size()) + ")");
    }
    else if ( data->GetString() == "SourceFile")
    {
        wxCommandEvent unusedEvent;
        OnEditSourceFileSelected(unusedEvent);
    }
}

/**
 * Right click on an item
 */
void ProjectManager::OnprojectsTreeItemRightClick(wxTreeEvent& event)
{
    selectedItem = event.GetItem();

    gdTreeItemProjectData * data = dynamic_cast<gdTreeItemProjectData*>(projectsTree->GetItemData(selectedItem));
    if ( data == NULL)
        return;

    if ( data->GetString() == "Scene")
    {
        PopupMenu(&sceneContextMenu);
    }
    else if ( data->GetString() == "Scenes")
    {
        PopupMenu(&scenesContextMenu);
    }
    else if ( data->GetString() == "ExternalEventsRoot")
    {
        PopupMenu(&emptyExternalEventsContextMenu);
    }
    else if ( data->GetString() == "ExternalEvents")
    {
        PopupMenu(&externalEventsContextMenu);
    }
    else if ( data->GetString() == "ExternalLayoutsRoot")
    {
        PopupMenu(&emptyExternalLayoutsContextMenu);
    }
    else if ( data->GetString() == "ExternalLayout")
    {
        PopupMenu(&externalLayoutContextMenu);
    }
    else if ( data->GetString() == "SourceFiles")
    {
        PopupMenu(&sourceFilesContextMenu);
    }
    else if ( data->GetString() == "SourceFile")
    {
        PopupMenu(&sourceFileContextMenu);
    }
    else if ( data->GetString() == "Root")
    {
        PopupMenu(&gameContextMenu);
    }
}

void ProjectManager::OnEditSourceFileSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    wxFileName filename(data->GetSecondString());
    filename.MakeAbsolute(wxFileName::FileName(game->GetProjectFile()).GetPath());

    EditSourceFile(game, ToString(filename.GetFullPath()));
}

void ProjectManager::EditSourceFile(Game * game, std::string filename, size_t line)
{
    if ( !wxFileExists(filename) )
    {
        wxLogWarning(_("Unable to open ")+filename+_(", the file does not exists"));
        return;
    }

    //Launch external source editor if one must be used
    bool useExternalEditor;
    wxConfigBase::Get()->Read("/Code/UseExternalEditor", &useExternalEditor, false);
    if ( useExternalEditor )
    {
        wxString program;
        wxConfigBase::Get()->Read("/Code/ExternalEditor", &program);

        wxExecute(program+" \""+filename+"\"");
        return;
    }
    //Launch an internal code editor else

    //Having a game associated with the editor is optional
    Game * associatedGame = NULL;
    if ( game )
    {
        vector< boost::shared_ptr<SourceFile> >::const_iterator sourceFile =
            find_if(game->externalSourceFiles.begin(), game->externalSourceFiles.end(), bind2nd(ExternalSourceFileHasName(), filename));

        if ( sourceFile != game->externalSourceFiles.end() ) associatedGame = game;
    }

    //Verify if the editor is not already opened
    for (unsigned int j =0;j<mainEditor.GetEditorsNotebook()->GetPageCount() ;j++ )
    {
        CodeEditor * editorPtr = dynamic_cast<CodeEditor*>(mainEditor.GetEditorsNotebook()->GetPage(j));

        if ( editorPtr != NULL && editorPtr->filename == filename )
        {
            //Change notebook page to scene page
            mainEditor.GetEditorsNotebook()->SetSelection(j);
            if ( line != std::string::npos ) editorPtr->SelectLine(line);
            return;
        }
    }

    CodeEditor * editorScene = new CodeEditor(mainEditor.GetEditorsNotebook(), filename, associatedGame, mainEditor.GetMainFrameWrapper());
    if ( !mainEditor.GetEditorsNotebook()->AddPage(editorScene, wxFileName(filename).GetFullName(), true, wxBitmap("res/source_cpp16.png", wxBITMAP_TYPE_ANY)) )
    {
        wxLogError(_("Unable to add a new tab !"));
    }
    if ( line != std::string::npos ) editorScene->SelectLine(line);
}

/**
 * Edit a scene
 */
void ProjectManager::OneditSceneMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) )
    {
        wxLogWarning(_("Choose a scene to edit in the project's manager"));
        return;
    }

    if ( !game->HasLayoutNamed(data->GetSecondString()) )
    {
        wxLogWarning(_("Scene not found."));
        return;
    }

    EditLayout(*game, game->GetLayout(data->GetSecondString()));
}

void ProjectManager::EditLayout(gd::Project & project, gd::Layout & layout)
{
    //Verify if the scene editor is not already opened
    for (unsigned int j =0;j<mainEditor.GetEditorsNotebook()->GetPageCount() ;j++ )
    {
        EditorScene * sceneEditorPtr = dynamic_cast<EditorScene*>(mainEditor.GetEditorsNotebook()->GetPage(j));

        if ( sceneEditorPtr != NULL && &sceneEditorPtr->GetLayout() == &layout )
        {
            //Change notebook page to the layout page
            mainEditor.GetEditorsNotebook()->SetSelection(j);
            return;
        }
    }

    //Save the event to log file
    LogFileManager::GetInstance()->WriteToLogFile("Opened layout "+layout.GetName());

    //Open a new editor if necessary
    string prefix = "";
    if ( mainEditor.games.size() > 1 )
    {
        prefix = "["+project.GetName()+"] ";
        if ( project.GetName().length() > gameMaxCharDisplayedInEditor )
            prefix = "["+project.GetName().substr(0, gameMaxCharDisplayedInEditor-3)+"...] ";
    }

    EditorScene * editorScene = new EditorScene(mainEditor.GetEditorsNotebook(), project, layout, mainEditor.GetMainFrameWrapper());
    if ( !mainEditor.GetEditorsNotebook()->AddPage(editorScene, prefix+layout.GetName(), true, wxBitmap("res/sceneeditor.png", wxBITMAP_TYPE_ANY)) )
    {
        wxLogError(_("Unable to add a new tab !"));
    }
}

/**
 * Edit properties of a scene
 */
void ProjectManager::OneditScenePropMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) )
    {
        wxLogWarning(_("Choose a scene to edit in the project's manager"));
        return;
    }

    if ( !game->HasLayoutNamed(data->GetSecondString()) )
    {
        wxLogWarning(_("Scene not found."));
        return;
    }

    gd::Layout & layout = game->GetLayout(data->GetSecondString());

    EditPropScene dialog( this, layout );
    dialog.ShowModal();
}

/**
 * Edit variables of a scene
 */
void ProjectManager::OnmodVarSceneMenuISelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) )
    {
        wxLogWarning(_("Choose a scene to edit in the project's manager"));
        return;
    }

    vector< boost::shared_ptr<Scene> >::const_iterator scene =
        find_if(game->GetLayouts().begin(), game->GetLayouts().end(), bind2nd(SceneHasName(), data->GetSecondString()));

    if ( scene == game->GetLayouts().end() )
    {
        wxLogWarning(_("Scene not found."));
        return;
    }

    gd::ChooseVariableDialog dialog(this, (*scene)->GetVariables(), /*editingOnly=*/true);
    dialog.SetAssociatedLayout(game, (*scene).get());
    if ( dialog.ShowModal() == 1 )
        game->GetChangesNotifier().OnVariablesModified(*game, (*scene).get());
}

/**
 * Edit name of a scene
 */
void ProjectManager::OneditSceneNameMenuItemSelected(wxCommandEvent& event)
{
    projectsTree->EditLabel( selectedItem );
}

/**
 * Start renaming something
 */
void ProjectManager::OnprojectsTreeBeginLabelEdit(wxTreeEvent& event)
{
    selectedItem = event.GetItem();

    itemTextBeforeEditing = projectsTree->GetItemText(event.GetItem());

    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    if ( data->GetString() != "Scene" && data->GetString() != "Root" && data->GetString() != "ExternalEvents"&& data->GetString() != "ExternalLayout")
        projectsTree->EndEditLabel( selectedItem, true );
}

/**
 * End renaming something
 */
void ProjectManager::OnprojectsTreeEndLabelEdit(wxTreeEvent& event)
{
    if ( event.IsEditCancelled() ) return;

    selectedItem = event.GetItem();
    string newName = string(event.GetLabel().mb_str());

    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    if ( data->GetString() == "Root")
    {
        game->SetName( newName );
    }
    //Renaming a scene
    else if ( data->GetString() == "Scene")
    {
        if ( !game->HasLayoutNamed(data->GetSecondString()) )
        {
            wxLogWarning(_("Scene not found."));
            return;
        }

        gd::Layout & layout = game->GetLayout(data->GetSecondString());

        if ( game->HasLayoutNamed(newName) )
        {
            wxLogWarning( _( "Unable to rename : A scene has already the same name." ) );
            Refresh();
            return;
        }

        projectsTree->SetItemData(selectedItem, new gdTreeItemProjectData("Scene", newName, game));

        layout.SetName(newName);

        //Updating editors
        for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
        {
            EditorScene * sceneEditorPtr = dynamic_cast<EditorScene*>(mainEditor.GetEditorsNotebook()->GetPage(k));

            if ( sceneEditorPtr != NULL && &sceneEditorPtr->GetLayout() == &layout)
                mainEditor.GetEditorsNotebook()->SetPageText(k, event.GetLabel());
        }

        game->GetChangesNotifier().OnLayoutRenamed(*game, layout, data->GetSecondString());
    }
    //Renaming external events
    else if ( data->GetString() == "ExternalEvents")
    {
        if ( !game->HasExternalEventsNamed(itemTextBeforeEditing) )
        {
            wxLogWarning(_("Unable to found events."));
            return;
        }

        if ( game->HasExternalEventsNamed(newName) )
        {
            wxLogWarning( _( "Unable to rename : Some external events have already the same name." ) );
            Refresh();
            return;
        }

        projectsTree->SetItemData(selectedItem, new gdTreeItemProjectData("ExternalEvents", newName, game));

        game->GetExternalEvents(itemTextBeforeEditing).SetName(newName);

        //Updating editors
        for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
        {
            ExternalEventsEditor * editorPtr = dynamic_cast<ExternalEventsEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));

            if ( editorPtr != NULL && &editorPtr->events == &game->GetExternalEvents(newName))
                mainEditor.GetEditorsNotebook()->SetPageText(k, event.GetLabel());
        }
    }
    //Renaming external layout
    else if ( data->GetString() == "ExternalLayout")
    {
        if ( !game->HasExternalLayoutNamed(itemTextBeforeEditing) )
        {
            wxLogWarning(_("Unable to found events."));
            return;
        }

        if ( game->HasExternalLayoutNamed(newName) )
        {
            wxLogWarning( _( "Unable to rename : Some external events have already the same name." ) );
            Refresh();
            return;
        }

        projectsTree->SetItemData(selectedItem, new gdTreeItemProjectData("ExternalLayout", newName, game));

        game->GetExternalLayout(itemTextBeforeEditing).SetName(newName);

        //Updating editors
        for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
        {
            ExternalLayoutEditor * editorPtr = dynamic_cast<ExternalLayoutEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));

            if ( editorPtr != NULL && &editorPtr->GetExternalLayout() == &game->GetExternalLayout(newName))
                mainEditor.GetEditorsNotebook()->SetPageText(k, event.GetLabel());
        }
    }
}

/**
 * Add a scene from ribbon button
 */
void ProjectManager::OnRibbonAddSceneSelected(wxRibbonButtonBarEvent& event)
{
    if ( !mainEditor.CurrentGameIsValid() ) return;

    AddLayoutToProject(mainEditor.GetCurrentGame().get(), mainEditor.GetCurrentGame()->GetLayoutCount());

    Refresh();
}

/**
 * Add a new scene to a game from Right Click
 */
void ProjectManager::OnaddSceneMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    AddLayoutToProject(game, game->GetLayoutCount());

    Refresh();
}

/**
 * Add a scene to a game
 */
void ProjectManager::AddLayoutToProject(gd::Project * project, unsigned int position)
{
    if ( !project ) return;

    //Finding a new, unique name for the scene
    string newSceneName = string(_("New scene"));
    int i = 2;
    while(project->HasLayoutNamed(newSceneName))
    {
        newSceneName = _("New scene") + " " + ToString(i);
        ++i;
    }

    project->InsertNewLayout(newSceneName, position);

    if ( project->HasLayoutNamed(newSceneName) )
        project->GetChangesNotifier().OnLayoutAdded(*project, project->GetLayout(newSceneName));
    else
        wxLogError(_("Unable to add the new layout!"));
}

/**
 * Edit images from ribbon button
 */
void ProjectManager::OnRibbonEditImagesSelected(wxRibbonButtonBarEvent& event)
{
    if ( !mainEditor.CurrentGameIsValid() ) return;

    EditResourcesOfProject(mainEditor.GetCurrentGame().get());
}

/**
 * Edit images of a game
 */
void ProjectManager::EditResourcesOfProject(gd::Project * project)
{
    //Verify if the image editor is not already opened
    for (unsigned int j =0;j<mainEditor.GetEditorsNotebook()->GetPageCount() ;j++ )
    {
        ResourcesEditor * imagesEditorPtr = dynamic_cast<ResourcesEditor*>(mainEditor.GetEditorsNotebook()->GetPage(j));

        if ( imagesEditorPtr != NULL && &imagesEditorPtr->project == project )
        {
            //Change notebook page to editor page
            mainEditor.GetEditorsNotebook()->SetSelection(j);
            return;
        }
    }

    //Open a new editor if necessary
    string prefix = "";
    if ( mainEditor.games.size() > 1 )
    {
        prefix = "["+project->GetName()+"] ";
        if ( project->GetName().length() > gameMaxCharDisplayedInEditor )
            prefix = "["+project->GetName().substr(0, gameMaxCharDisplayedInEditor-3)+"...] ";
    }

    ResourcesEditor * editorImages = new ResourcesEditor(&mainEditor, *project, mainEditor.GetMainFrameWrapper(), true);
    mainEditor.GetEditorsNotebook()->AddPage(editorImages, prefix+_("Images bank"), true, wxBitmap("res/imageicon.png", wxBITMAP_TYPE_ANY));
}

/**
 * Delete a scene from a game
 */
void ProjectManager::OndeleteSceneMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    std::string sceneName = data->GetSecondString();
    if ( !game->HasLayoutNamed(sceneName) )
    {
        wxLogWarning(_("Scene not found."));
        return;
    }

    gd::Layout & layout = game->GetLayout(sceneName);

    //Updating editors
    for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
    {
        EditorScene * sceneEditorPtr = dynamic_cast<EditorScene*>(mainEditor.GetEditorsNotebook()->GetPage(k));
        ExternalLayoutEditor * externalLayoutEditPtr = dynamic_cast<ExternalLayoutEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));

        if ( (sceneEditorPtr != NULL && &sceneEditorPtr->GetLayout() == &layout) ||
             (externalLayoutEditPtr != NULL && &externalLayoutEditPtr->GetAssociatedLayout() == &layout) )
        {
            if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );

            k--;
        }
    }

    //Updating tree
    projectsTree->Delete(selectedItem);

    //Ensure we're not destroying a scene with events being built
    wxBusyInfo * waitDialog = CodeCompiler::GetInstance()->CompilationInProcess() ? new wxBusyInfo("Veuillez patienter, la compilation interne des �v�nements de la sc�ne\ndoit �tre men�e � terme avant de supprimer la sc�ne...") : NULL;
    while ( CodeCompiler::GetInstance()->CompilationInProcess() )
    {
        wxYield();
    }
    if ( waitDialog ) delete waitDialog;

    game->RemoveLayout(sceneName);
    game->GetChangesNotifier().OnLayoutDeleted(*game, sceneName);
}

/**
 * Copy a scene
 */
void ProjectManager::OncopySceneMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    if ( !game->HasLayoutNamed(data->GetSecondString()) )
    {
        wxLogWarning(_("Scene not found."));
        return;
    }

    gd::Clipboard::GetInstance()->SetLayout(&game->GetLayout(data->GetSecondString()));
}

/**
 * Cut a scene
 */
void ProjectManager::OncutSceneMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    std::string layoutName = data->GetSecondString();
    if ( !game->HasLayoutNamed(layoutName) )
    {
        wxLogWarning(_("Scene not found."));
        return;
    }

    gd::Layout & layout = game->GetLayout(layoutName);

    gd::Clipboard::GetInstance()->SetLayout(&layout);

    //Updating editors
    for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
    {
        EditorScene * sceneEditorPtr = dynamic_cast<EditorScene*>(mainEditor.GetEditorsNotebook()->GetPage(k));

        if ( sceneEditorPtr != NULL && &sceneEditorPtr->GetLayout() == &layout)
        {
            if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );

            k--;
        }
    }

    //Updating tree
    projectsTree->Delete(selectedItem);

    //Ensure we're not destroying a scene with events being built
    wxBusyInfo * waitDialog = CodeCompiler::GetInstance()->CompilationInProcess() ? new wxBusyInfo("Veuillez patienter, la compilation interne des �v�nements\ndoit �tre men�e � terme avant de continuer...") : NULL;
    while (CodeCompiler::GetInstance()->CompilationInProcess())
    {
        wxYield();
    }
    if ( waitDialog ) delete waitDialog;

    game->RemoveLayout(layoutName);
    game->GetChangesNotifier().OnLayoutDeleted(*game, layoutName);
}

void ProjectManager::OnpasteSceneMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    gd::Clipboard * clipboard = gd::Clipboard::GetInstance();
    if (!clipboard->HasLayout()) return;

    gd::Layout & newLayout = *clipboard->GetLayout();

    //Finding a new, unique name for the layout
    string newLayoutName = string(_("Copy of")) + " " + newLayout.GetName();
    int i = 2;
    while(game->HasLayoutNamed(newLayoutName))
    {
        newLayoutName = _("Copy of") + " " + newLayout.GetName() + " " + ToString(i);
        ++i;
    }

    newLayout.SetName(newLayoutName);
    game->InsertLayout(newLayout, game->GetLayoutPosition(data->GetSecondString()));
    game->GetChangesNotifier().OnLayoutAdded(*game, game->GetLayout(newLayoutName));

    //Insert in tree
    gdTreeItemProjectData * sceneItemData = new gdTreeItemProjectData("Scene", newLayoutName, game);
    if ( projectsTree->GetPrevSibling(selectedItem).IsOk() )
        projectsTree->InsertItem(projectsTree->GetItemParent(selectedItem), projectsTree->GetPrevSibling(selectedItem), newLayoutName, 1, 1, sceneItemData);
    else
        projectsTree->InsertItem(projectsTree->GetItemParent(selectedItem), 0, newLayoutName, 1, 1, sceneItemData);
}

/**
 * Edit the name of a game
 */
void ProjectManager::OneditNameGameMenuItemSelected(wxCommandEvent& event)
{
    projectsTree->EditLabel( selectedItem );
}

/**
 * Edit global variables of a game
 */
void ProjectManager::OneditGblVarMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    gd::ChooseVariableDialog dialog(this, game->GetVariables(), /*editingOnly=*/true);
    dialog.SetAssociatedProject(game);
    if ( dialog.ShowModal() == 1 )
    {
        game->GetChangesNotifier().OnVariablesModified(*game);
    }
}

/**
 * Edit the properties of a game
 */
void ProjectManager::OneditPropGameMenuItemSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    mainEditor.GetProjectPropertiesPanel()->SetProject(game);
    mainEditor.GetProjectPropertiesPanel()->SetAssociatedTreeCtrlProjectItem(projectsTree, selectedItem);
    mainEditor.GetProjectPropertiesPanel()->SetAssociatedProjectManager(this);
    mainEditor.GetAUIPaneManger()->GetPane("PP").Show(true);
    mainEditor.GetAUIPaneManger()->Update();
}

/**
 * Edit extensions used by a project
 */
void ProjectManager::EditExtensionsOfGame(gd::Project & project)
{
    gd::ProjectExtensionsDialog dialog(this, project);
    dialog.ShowModal();
}

void ProjectManager::OnRibbonExtensionsSelected(wxRibbonButtonBarEvent& event)
{
    if ( !mainEditor.CurrentGameIsValid() ) return;
    EditExtensionsOfGame(*mainEditor.GetCurrentGame());

    Refresh();
}

/**
 * Close a game
 */
void ProjectManager::CloseGame(gd::Project * project)
{
    //Closing all editors related to game
    for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
    {
        EditorScene * sceneEditorPtr = dynamic_cast<EditorScene*>(mainEditor.GetEditorsNotebook()->GetPage(k));
        ExternalEventsEditor * externalEventsEditorPtr = dynamic_cast<ExternalEventsEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));
        ResourcesEditor * imagesEditorPtr = dynamic_cast<ResourcesEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));
        CodeEditor * codeEditorPtr = dynamic_cast<CodeEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));
        ExternalLayoutEditor * externalLayoutEditorPtr = dynamic_cast<ExternalLayoutEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));


        if ( sceneEditorPtr != NULL )
        {
            bool sceneBelongToGame = false;
            for (unsigned int i = 0;i<project->GetLayoutCount();++i)
            {
            	if ( &project->GetLayout(i) == &sceneEditorPtr->GetLayout() )
                    sceneBelongToGame = true;
            }

            if ( sceneBelongToGame )
            {
                if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                    wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );
                k--;
            }
        }
        else if ( imagesEditorPtr != NULL )
        {
            if ( &imagesEditorPtr->project == project)
            {
                if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                    wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );
                k--;
            }
        }
        else if ( externalEventsEditorPtr != NULL )
        {
            if ( &externalEventsEditorPtr->game == project)
            {
                if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                    wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );
                k--;
            }
        }
        else if ( codeEditorPtr != NULL )
        {
            if ( codeEditorPtr->game == project)
            {
                if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                    wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );
                k--;
            }
        }
        else if ( externalLayoutEditorPtr != NULL )
        {
            if ( &externalLayoutEditorPtr->GetProject() == project)
            {
                if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                    wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );
                k--;
            }
        }
    }
    if ( mainEditor.GetProjectPropertiesPanel()->GetProject() == project ) mainEditor.GetProjectPropertiesPanel()->SetProject(NULL);

    //Ensure we're not destroying a scene with events being built
    wxBusyInfo * waitDialog = CodeCompiler::GetInstance()->CompilationInProcess() ? new wxBusyInfo("Veuillez patienter, la compilation interne des �v�nements de la sc�ne\ndoit �tre men�e � terme avant de fermer le jeu...") : NULL;
    while ( CodeCompiler::GetInstance()->CompilationInProcess() )
    {
        wxYield();
    }
    if ( waitDialog ) delete waitDialog;

    for (unsigned int i = 0;i<mainEditor.games.size();++i)
    {
    	if ( mainEditor.games[i].get() == project)
            mainEditor.games.erase(mainEditor.games.begin()+i);
    }

    mainEditor.SetCurrentGame(mainEditor.games.size()-1, /*refreshProjectManager=*/false);
}

/**
 * Click on Close Button in ribbon
 */
void ProjectManager::OnRibbonCloseSelected(wxRibbonButtonBarEvent& event)
{
    if ( !mainEditor.CurrentGameIsValid() ) return;

    if ( wxMessageBox( _( "Warning! All changes not saved will be lost.\n\nClose the project \?" ), _( "Confirmation" ), wxYES_NO, this ) == wxNO )
        return;

    CloseGame(mainEditor.GetCurrentGame().get());

    Refresh();
}

/**
 * Right-click > Close
 */
void ProjectManager::OncloseGameBtSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    if ( wxMessageBox( _( "Warning! All changes not saved will be lost.\n\nClose the project \?" ), _( "Confirmation" ), wxYES_NO, this ) == wxNO )
        return;

    CloseGame(game);

    projectsTree->Delete(selectedItem);
}

void ProjectManager::OnprojectsTreeSelectionChanged(wxTreeEvent& event)
{
    selectedItem = event.GetItem();
    mainEditor.GetRibbon()->SetActivePage(static_cast<size_t>(0));
}

/**
 * Edit anything selected in the tree
 */
void ProjectManager::OnRibbonEditSelectionSelected(wxRibbonButtonBarEvent& event)
{
    wxTreeEvent unusedEvent;
    OnprojectsTreeItemActivated(unusedEvent);
}

/**
 * Edit external events
 */
void ProjectManager::OnEditExternalEventsSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    if ( !game->HasExternalEventsNamed(data->GetSecondString()) )
    {
        wxLogWarning(_("Unable to found external events."));
        return;
    }

    //Verify if the scene editor is not already opened
    for (unsigned int j =0;j<mainEditor.GetEditorsNotebook()->GetPageCount() ;j++ )
    {
        ExternalEventsEditor * eventsEditorPtr = dynamic_cast<ExternalEventsEditor*>(mainEditor.GetEditorsNotebook()->GetPage(j));

        if ( eventsEditorPtr != NULL && &eventsEditorPtr->events == &game->GetExternalEvents(data->GetSecondString()) )
        {
            //Change notebook page to scene page
            mainEditor.GetEditorsNotebook()->SetSelection(j);
            return;
        }
    }

    //Open a new editor if necessary
    string prefix = "";
    if ( mainEditor.games.size() > 1 )
    {
        prefix = "["+game->GetName()+"] ";
        if ( game->GetName().length() > gameMaxCharDisplayedInEditor )
            prefix = "["+game->GetName().substr(0, gameMaxCharDisplayedInEditor-3)+"...] ";
    }

    ExternalEventsEditor * editor = new ExternalEventsEditor(mainEditor.GetEditorsNotebook(), *game, game->GetExternalEvents(data->GetSecondString()), mainEditor.GetMainFrameWrapper());
    if ( !mainEditor.GetEditorsNotebook()->AddPage(editor, prefix+data->GetSecondString(), true, wxBitmap("res/events16.png", wxBITMAP_TYPE_ANY)) )
    {
        wxLogError(_("Unable to add a new tab !"));
    }
}

/**
* Add external events from right click
*/
void ProjectManager::OnAddExternalEventsSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    AddExternalEventsToGame(game);

    Refresh();
}


/**
 * Add external events from ribbon button
 */
void ProjectManager::OnRibbonAddExternalEventsSelected(wxRibbonButtonBarEvent& event)
{
    if ( !mainEditor.CurrentGameIsValid() ) return;

    AddExternalEventsToGame(mainEditor.GetCurrentGame().get());

    Refresh();
}

/**
 * Add external events to a game
 */
void ProjectManager::AddExternalEventsToGame(gd::Project * project)
{
    //Finding a new, unique name for the scene
    string newName = string(_("External events"));
    int i = 2;
    while(project->HasExternalEventsNamed(newName))
    {
        newName = _("External events") + " " + ToString(i);
        ++i;
    }

    project->InsertNewExternalEvents(newName, project->GetExternalEventsCount());
}

void ProjectManager::OnRenameExternalEventsSelected(wxCommandEvent& event)
{
    projectsTree->EditLabel( selectedItem );
}

void ProjectManager::OnDeleteExternalEventsSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    std::string externalEventsName = data->GetSecondString();
    if ( !game->HasExternalEventsNamed(externalEventsName) )
    {
        wxLogWarning(_("Unable to found events."));
        return;
    }

    //Updating editors
    for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
    {
        ExternalEventsEditor * editorPtr = dynamic_cast<ExternalEventsEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));

        if ( editorPtr != NULL && &editorPtr->events == &game->GetExternalEvents(data->GetSecondString()))
        {
            if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );

            k--;
        }
    }

    //Updating tree
    projectsTree->Delete(selectedItem);

    game->RemoveExternalEvents(externalEventsName);
}

void ProjectManager::OnCopyExternalEventsSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    if ( !game->HasExternalEventsNamed(data->GetSecondString()) )
    {
        wxLogWarning(_("Unable to found events."));
        return;
    }

    gd::Clipboard::GetInstance()->SetExternalEvents(&game->GetExternalEvents(data->GetSecondString()));
}

void ProjectManager::OnCutExternalEventsSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    std::string externalEventsName = data->GetSecondString();
    if ( !game->HasExternalEventsNamed(externalEventsName) )
    {
        wxLogWarning(_("Unable to found events."));
        return;
    }

    gd::Clipboard::GetInstance()->SetExternalEvents(&game->GetExternalEvents(data->GetSecondString()));

    //Updating editors
    for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
    {
        ExternalEventsEditor * editorPtr = dynamic_cast<ExternalEventsEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));

        if ( editorPtr != NULL && &editorPtr->events == &game->GetExternalEvents(data->GetSecondString()))
        {
            if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );

            k--;
        }
    }

    //Updating tree
    projectsTree->Delete(selectedItem);

    game->RemoveExternalEvents(externalEventsName);
}

void ProjectManager::OnPasteExternalEventsSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    gd::ExternalEvents & newEvents = *Clipboard::GetInstance()->GetExternalEvents();

    //Finding a new, unique name for the events
    string newName = string(_("Copy of")) + " " + newEvents.GetName();
    int i = 2;
    while(game->HasExternalEventsNamed(newName))
    {
        newName = _("Copy of") + " " + newEvents.GetName() + " " + ToString(i);
        ++i;
    }

    newEvents.SetName(newName);
    game->InsertExternalEvents(newEvents, game->GetExternalEventsPosition(data->GetSecondString()));

    //Insert in tree
    gdTreeItemProjectData * eventsItemData = new gdTreeItemProjectData("ExternalEvents", newName, game);
    if ( projectsTree->GetPrevSibling(selectedItem).IsOk() )
        projectsTree->InsertItem(projectsTree->GetItemParent(selectedItem), projectsTree->GetPrevSibling(selectedItem), newName, 4, 4, eventsItemData);
    else
        projectsTree->InsertItem(projectsTree->GetItemParent(selectedItem), 0, newName, 4, 4, eventsItemData);
}

/**
 * Add external layout to a game
 */
void ProjectManager::AddExternalLayoutToGame(gd::Project * project)
{
    //Finding a new, unique name for the scene
    string newName = string(_("External layout"));
    int i = 2;
    while(project->HasExternalLayoutNamed(newName))
    {
        newName = _("External layout") + " " + ToString(i);
        ++i;
    }

    project->InsertNewExternalLayout(newName, project->GetExternalLayoutsCount());
}

void ProjectManager::OnAddExternalLayoutSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    AddExternalLayoutToGame(game);

    Refresh();
}

/**
 * Add external layout from ribbon button
 */
void ProjectManager::OnRibbonAddExternalLayoutSelected(wxRibbonButtonBarEvent& event)
{
    if ( !mainEditor.CurrentGameIsValid() ) return;

    AddExternalLayoutToGame(mainEditor.GetCurrentGame().get());

    Refresh();
}

void ProjectManager::OnEditExternalLayoutSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    if ( !game->HasExternalLayoutNamed(data->GetSecondString()) )
    {
        wxLogWarning(_("Unable to found external events."));
        return;
    }

    //Verify if the scene editor is not already opened
    for (unsigned int j =0;j<mainEditor.GetEditorsNotebook()->GetPageCount() ;j++ )
    {
        ExternalLayoutEditor * externalLayoutEditorPtr = dynamic_cast<ExternalLayoutEditor*>(mainEditor.GetEditorsNotebook()->GetPage(j));

        if ( externalLayoutEditorPtr != NULL && &externalLayoutEditorPtr->GetExternalLayout() == &game->GetExternalLayout(data->GetSecondString()) )
        {
            //Change notebook page to scene page
            mainEditor.GetEditorsNotebook()->SetSelection(j);
            return;
        }
    }

    //Open a new editor if necessary
    string prefix = "";
    if ( mainEditor.games.size() > 1 )
    {
        prefix = "["+game->GetName()+"] ";
        if ( game->GetName().length() > gameMaxCharDisplayedInEditor )
            prefix = "["+game->GetName().substr(0, gameMaxCharDisplayedInEditor-3)+"...] ";
    }

    ExternalLayoutEditor * editor = new ExternalLayoutEditor(mainEditor.GetEditorsNotebook(), *game, game->GetExternalLayout(data->GetSecondString()), mainEditor.GetMainFrameWrapper());
    if ( !mainEditor.GetEditorsNotebook()->AddPage(editor, prefix+data->GetSecondString(), true, wxBitmap("res/sceneeditor.png", wxBITMAP_TYPE_ANY)) )
    {
        wxLogError(_("Unable to add a new tab !"));
    }
}

void ProjectManager::OnRenameExternalLayoutSelected(wxCommandEvent& event)
{
    projectsTree->EditLabel( selectedItem );
}

void ProjectManager::OnDeleteExternalLayoutSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    std::string externalLayoutName = data->GetSecondString();
    if ( !game->HasExternalLayoutNamed(externalLayoutName) )
    {
        wxLogWarning(_("Unable to found external layout."));
        return;
    }

    //Updating editors
    for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
    {
        ExternalLayoutEditor * editorPtr = dynamic_cast<ExternalLayoutEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));

        if ( editorPtr != NULL && &editorPtr->GetExternalLayout() == &game->GetExternalLayout(data->GetSecondString()))
        {
            if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );

            k--;
        }
    }

    //Updating tree
    projectsTree->Delete(selectedItem);

    game->RemoveExternalLayout(externalLayoutName);
}

void ProjectManager::OnCopyExternalLayoutSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    if ( !game->HasExternalLayoutNamed(data->GetSecondString()) )
    {
        wxLogWarning(_("Unable to found external layout."));
        return;
    }

    gd::Clipboard::GetInstance()->SetExternalLayout(&game->GetExternalLayout(data->GetSecondString()));
}

void ProjectManager::OnCutExternalLayoutSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    std::string externalLayoutName = data->GetSecondString();
    if ( !game->HasExternalLayoutNamed(externalLayoutName) )
    {
        wxLogWarning(_("Unable to found external layout."));
        return;
    }

    gd::Clipboard::GetInstance()->SetExternalLayout(&game->GetExternalLayout(data->GetSecondString()));

    //Updating editors
    for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
    {
        ExternalLayoutEditor * editorPtr = dynamic_cast<ExternalLayoutEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));

        if ( editorPtr != NULL && &editorPtr->GetExternalLayout() == &game->GetExternalLayout(data->GetSecondString()))
        {
            if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );

            k--;
        }
    }

    //Updating tree
    projectsTree->Delete(selectedItem);

    game->RemoveExternalLayout(externalLayoutName);
}

void ProjectManager::OnPasteExternalLayoutSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    gd::ExternalLayout & newExternalLayout = *Clipboard::GetInstance()->GetExternalLayout();

    //Finding a new, unique name for the events
    string newName = string(_("Copy of")) + " " + newExternalLayout.GetName();
    int i = 2;
    while(game->HasExternalLayoutNamed(newName))
    {
        newName = _("Copy of") + " " + newExternalLayout.GetName() + " " + ToString(i);
        ++i;
    }

    newExternalLayout.SetName(newName);
    game->InsertExternalLayout(newExternalLayout, game->GetExternalLayoutPosition(data->GetSecondString()));

    //Insert in tree
    gdTreeItemProjectData * eventsItemData = new gdTreeItemProjectData("ExternalLayout", newName, game);
    if ( projectsTree->GetPrevSibling(selectedItem).IsOk() )
        projectsTree->InsertItem(projectsTree->GetItemParent(selectedItem), projectsTree->GetPrevSibling(selectedItem), newName, 6, 6, eventsItemData);
    else
        projectsTree->InsertItem(projectsTree->GetItemParent(selectedItem), 0, newName, 6, 6, eventsItemData);
}

void ProjectManager::OnAddCppSourceFileSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    wxFileDialog fileDialog( this, _("Choose one or more files to add"), "", "", _("C++ sources|*.cpp;*.cxx|All files|*.*"), wxFD_MULTIPLE );
    if ( fileDialog.ShowModal() != wxID_OK ) return;

    wxArrayString files;
    wxArrayString names;
    fileDialog.GetFilenames( names );
    fileDialog.GetPaths( files );

    for ( unsigned int i = 0; i < files.GetCount();i++ )
    {
        boost::shared_ptr<SourceFile> sourceFile(new SourceFile);
        sourceFile->SetFileName(string(files[i].mb_str()));

        game->externalSourceFiles.push_back(sourceFile);
    }

    Refresh();
}

void ProjectManager::OnDeleteSourceFileSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    vector< boost::shared_ptr<SourceFile> >::iterator sourceFile =
        find_if(game->externalSourceFiles.begin(), game->externalSourceFiles.end(), bind2nd(ExternalSourceFileHasName(), data->GetSecondString()));

    if ( sourceFile == game->externalSourceFiles.end() )
    {
        wxLogWarning(_("File not found"));
        return;
    }

    //Updating editors
    for (unsigned int k =0;k<static_cast<unsigned>(mainEditor.GetEditorsNotebook()->GetPageCount()) ;k++ )
    {
        CodeEditor * editorPtr = dynamic_cast<CodeEditor*>(mainEditor.GetEditorsNotebook()->GetPage(k));

        if ( editorPtr != NULL && editorPtr->filename == (*sourceFile)->GetFileName())
        {
            if ( !mainEditor.GetEditorsNotebook()->DeletePage(k) )
                wxMessageBox(_("Unable to delete a tab !"), _("Error"), wxICON_ERROR );

            k--;
        }
    }

    game->externalSourceFiles.erase(sourceFile);

    //Updating tree
    projectsTree->Delete(selectedItem);
}

void ProjectManager::OnCreateNewCppFileSelected(wxCommandEvent& event)
{
    RuntimeGame * game;
    gdTreeItemProjectData * data;
    if ( !GetGameOfSelectedItem(game, data) ) return;

    wxFileDialog dialog( this, _( "Choose a file name" ), "", "", "Source C++ (*.cpp)|*.cpp|Header C++ (*.h)|*.h", wxFD_SAVE|wxFD_OVERWRITE_PROMPT );
    if ( dialog.ShowModal() == wxID_CANCEL )
        return;

    //Creating an empty file
    std::ofstream file;
    file.open ( ToString(dialog.GetPath()).c_str() );
    file << "\n";
    file.close();

    //Adding it to the game source files.
    boost::shared_ptr<SourceFile> sourceFile(new SourceFile);

    wxFileName filename(dialog.GetPath()); //Files are added with their paths relative to the project directory
    filename.MakeRelativeTo(wxFileName::FileName(game->GetProjectFile()).GetPath());
    sourceFile->SetFileName(ToString(filename.GetFullPath()));

    vector< boost::shared_ptr<SourceFile> >::iterator alreadyExistingSourceFile =
        find_if(game->externalSourceFiles.begin(), game->externalSourceFiles.end(), bind2nd(ExternalSourceFileHasName(), ToString(filename.GetFullPath())));

    if ( alreadyExistingSourceFile == game->externalSourceFiles.end() )
        game->externalSourceFiles.push_back(sourceFile);
    Refresh();
}

/**
 * Display help
 */
void ProjectManager::OnRibbonHelpSelected(wxRibbonButtonBarEvent& event)
{
    gd::HelpFileAccess::GetInstance()->OpenURL(_("http://www.wiki.compilgames.net/doku.php/en/game_develop/documentation"));
}

