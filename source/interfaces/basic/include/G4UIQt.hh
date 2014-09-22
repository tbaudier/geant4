//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id$
//
#ifndef G4UIQt_h
#define G4UIQt_h 

#if defined(G4UI_BUILD_QT_SESSION) || defined(G4UI_USE_QT)

#include <map>

#include "G4VBasicShell.hh"
#include "G4VInteractiveSession.hh"

#include <qobject.h>
#include <qmap.h>
#include <qstringlist.h>
#include <qtabwidget.h>
#include <qdockwidget.h>

class QMainWindow;
class QLineEdit;
class G4UIsession;
class QListWidget;
class QTreeWidget;
class QTreeWidgetItem;
class QTextEdit;
class QLabel;
class QResizeEvent;
class QTabWidget;
class QStringList;
class QSplitter;
class QToolBar;
class QTableWidget;
class QPixmap;
class QComboBox;

// Class description :
//
//  G4UIQt : class to handle a Qt interactive session.
// G4UIQt is the Qt version of G4UIterminal.
//
//  A command box is at disposal for entering/recalling Geant4 commands.
//  A menubar could be customized through the AddMenu, AddButton, AddIcon methods.
//  Note that there are corresponding Geant4 commands to add a 
// menus in the menubar and add buttons in a menu.
//  Ex : 
//    /gui/addMenu   test Test
//    /gui/addButton test Init /run/initialize
//    /gui/addButton test "Set gun" "/control/execute gun.g4m"
//    /gui/addButton test "Run one event" "/run/beamOn 1"
//
//  Command completion, by typing "tab" key, is available on the 
// command line.
//
// Class description - end :

class G4QTabWidget : public QTabWidget {
public :
  G4QTabWidget();
  G4QTabWidget(QWidget* aParent, int sizeX, int sizeY);
  void paintEvent  ( QPaintEvent * event );
  inline void setTabSelected(bool a) { fTabSelected = a; };
  inline void setLastTabCreated(int a) { fLastCreated = a; };
  inline bool isTabSelected() { return fTabSelected; };
  bool fTabSelected;
  int fLastCreated;
  int fPreferedSizeX;
  int fPreferedSizeY;
  inline void setPreferredSize(QSize s) {
    fPreferedSizeX = s.width() + 6; // tab label height + margin left+right
    fPreferedSizeY = s.height() + 58; // margin left+right
  }
  inline QSize sizeHint () const {
    return QSize(fPreferedSizeX, fPreferedSizeY);
  }
};

class G4UIOutputString {
  public :
  G4UIOutputString(QString text,G4String thread = "",G4String outputstream= "info");
  inline QString GetOutputList() { return " all info warning error ";};
  QString fText;
  G4String fThread;
  G4String fOutputStream; // Error, Warning, Info
};


class G4UIDockWidget : public QDockWidget {
public:
  G4UIDockWidget(QString txt);
  void closeEvent(QCloseEvent *);
};


class G4UIQt : public QObject, public G4VBasicShell, public G4VInteractiveSession {
  Q_OBJECT

public: // With description
  G4UIQt(int,char**);
  // (argv, argc) or (0, NULL) had to be given.
  G4UIsession* SessionStart();
  // To enter interactive X loop ; waiting/executing command,...
  void AddMenu(const char*,const char*);
  // To add a pulldown menu in the menu bar. 
  // First argument is the name of the menu.
  // Second argument is the label of the cascade button.
  // Ex : AddMenu("my_menu","My menu")
  void AddButton(const char*,const char*,const char*);
  // To add a push button in a pulldown menu.
  // First argument is the name of the menu.
  // Second argument is the label of the button.
  // Third argument is the Geant4 command executed when the button is fired.
  // Ex : AddButton("my_menu","Run","/run/beamOn 1"); 
  void AddIcon(const char* userLabel, const char* iconFile, const char* command, const char* file_name="");
  // To add a icon in the toolbar
  // First argument is the label of the icon.
  // Second argument is the selected icon type (open save move rotate pick zoom_in zoom_out wireframe solid hidden_line_removal hidden_line_and_surface_removal perspective ortho user_icon).
  // Third argument is the Geant4 command executed when the button is fired.
  // Fourth argument is the path to the icon file if "user_icon" selected
  // Ex : AddButton("change background color","../background.xpm"," /vis/viewer/set/background"); 

  bool AddTabWidget(QWidget*,QString,int,int);
  // To add a tab for vis openGL Qt driver
  
  QTabWidget* GetSceneTreeComponentsTBWidget();
  // Get the viewComponent

  bool IsSplitterReleased();

  inline bool IsIconMoveSelected() {
    return fMoveSelected;
  };
  inline bool IsIconRotateSelected() {
    return fRotateSelected;
  };
  inline bool IsIconPickSelected() {
    return fPickSelected;
  };
  inline bool IsIconZoomInSelected() {
    return fZoomInSelected;
  };
  inline bool IsIconZoomOutSelected() {
    return fZoomOutSelected;
  };

  void SetIconMoveSelected();
  void SetIconRotateSelected();
  void SetIconPickSelected();
  void SetIconZoomInSelected();
  void SetIconZoomOutSelected();
  void SetIconHLHSRSelected();
  void SetIconHLRSelected();
  void SetIconSolidSelected();
  void SetIconWireframeSelected();
  void SetIconPerspectiveSelected();
  void SetIconOrthoSelected();

  inline QMainWindow * GetMainWindow() {
    return fMainWindow;
  };
  
  inline QPixmap* getSearchIcon() { return fSearchIcon;};
  // return the "search" icon pixmap
  inline QPixmap* getClearIcon() { return fClearIcon;};
  // return the "clear" icon pixmap

  void SetViewerFirstPageHTMLText(const std::string&);
  // Set the HTML text on the first page of the viewer. If "", will take the last value as default
  
public:
  ~G4UIQt();
  void Prompt(G4String);
  void SessionTerminate();
  virtual void PauseSessionStart(const G4String&);
  virtual G4int ReceiveG4cout(const G4String&);
  virtual G4int ReceiveG4cerr(const G4String&);
  //   G4String GetCommand(Widget);

private:
  void SecondaryLoop(G4String); // a VIRER
  void CreateHelpWidget();
  void InitHelpTreeAndVisParametersWidget();
  void FillHelpTree();
  virtual void ExitHelp() const;

  void CreateHelpTree(QTreeWidgetItem*,G4UIcommandTree*);
  QTreeWidgetItem* FindTreeItem(QTreeWidgetItem *,const QString&);

  QString GetCommandList(const G4UIcommand*);
  void updateHelpArea(const G4UIcommand*);
  virtual G4bool GetHelpChoice(G4int&);// have to be implemeted because we heritate from G4VBasicShell
  bool eventFilter(QObject*,QEvent*);
  void ActivateCommand(G4String);
  QMap<int,QString> LookForHelpStringInChildTree(G4UIcommandTree *,const QString&);

  QWidget* CreateVisParametersTBWidget();
  QWidget* CreateHelpTBWidget();
  G4UIDockWidget* CreateCoutTBWidget();
  QWidget* CreateHistoryTBWidget();
  G4UIDockWidget* CreateUITabWidget();
  QWidget* CreateSceneTreeComponentsTBWidget();
  QWidget* CreateViewerWidget();
  void OpenHelpTreeOnCommand(const QString &);
  QString GetShortCommandPath(QString);
  QString GetLongCommandPath(QTreeWidgetItem*);
  G4bool IsGUICommand(const G4UIcommand*);
  bool CreateVisCommandGroupAndToolBox(G4UIcommand*, QWidget*, int, bool isDialog);
  bool CreateCommandWidget(G4UIcommand* command, QWidget* parent, bool isDialog);
#ifdef G4MULTITHREADED
  void UpdateCoutThreadFilter();
#endif
  void FilterAllOutputTextArea();
  QString FilterOutput(const G4UIOutputString&,const QString&,const QString&);
  G4String GetThreadPrefix();

private:

  QMainWindow * fMainWindow;
  QLabel *fCommandLabel;
  QLineEdit * fCommandArea;
  QTextEdit *fCoutTBTextArea;
  QTabWidget* fUITabWidget;
  std::vector <G4UIOutputString> fG4OutputString;
  QLineEdit * fCoutFilter;

  QListWidget *fHistoryTBTableList;
  QTreeWidget *fHelpTreeWidget;
  QWidget* fHelpTBWidget;
  QWidget* fHistoryTBWidget;
  G4UIDockWidget* fCoutDockWidget;
  G4UIDockWidget* fUIDockWidget;
  QTabWidget* fSceneTreeComponentsTBWidget;
  QLineEdit* fHelpLine;
  G4QTabWidget* fViewerTabWidget;
  QString fCoutText;
  QLabel *fEmptyViewerWidget;
  QSplitter * fHelpVSplitter;
  QWidget* fViewerTabHandleWidget;
  QTextEdit* fParameterHelpLabel;
  QTableWidget* fParameterHelpTable;

  QToolBar *fToolbarApp;
  QToolBar *fToolbarUser;
  QString fStringSeparator;
  G4String fLastErrMessage;
  QString fLastOpenPath;
  
  QPixmap* fSearchIcon;
  QPixmap* fClearIcon;
  QComboBox* fThreadsFilterComboBox;
  std::string fDefaultViewerFirstPageHTMLText;
  
  bool fMoveSelected;
  bool fRotateSelected;
  bool fPickSelected;
  bool fZoomInSelected;
  bool fZoomOutSelected;

private Q_SLOTS :
  void ExitSession();
  void ClearButtonCallback();
  void CommandEnteredCallback();
  void CommandEditedCallback(const QString & text);
  void ButtonCallback(const QString&);
  void HelpTreeClicCallback();
  void HelpTreeDoubleClicCallback();
  void ShowHelpCallback();
  void CommandHistoryCallback();
  void LookForHelpStringCallback();
  void UpdateTabWidget(int);
  void ResizeTabWidget( QResizeEvent* );
  void CoutFilterCallback(const QString&);
  void ThreadComboBoxCallback(int);
  void TabCloseCallback(int);
  void ToolBoxActivated(int);
  void VisParameterCallback(QWidget*);
  void ChangeColorCallback(QWidget*);
  void ChangeCursorStyle(const QString&);
  void ChangeSurfaceStyle(const QString&);
  void OpenIconCallback(const QString&);
  void SaveIconCallback(const QString&);
  void ChangePerspectiveOrtho(const QString&);
};

#endif

#endif

