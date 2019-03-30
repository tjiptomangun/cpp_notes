; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=Play
LastTemplate=CEdit
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Ray.h"
LastPage=0

ClassCount=14
Class1=CRayApp
Class2=CRayDoc
Class3=CRayView
Class4=CMainFrame

ResourceCount=6
Resource1=IDR_WIZBAR
Resource2=IDD_ABOUTBOX
Resource3=IDR_MAINFRAME
Class5=CChildFrame
Class6=CAboutDlg
Resource4=ID_RENDERSCENE
Class7=RenderWindow
Resource5=IDR_RAYTYPE
Class8=Play
Class9=WindFromX
Class10=WindFromY
Class11=WindFromZ
Class12=WindAtX
Class13=WindAtY
Class14=WindAtZ
Resource6=IDD_PLAY

[CLS:CRayApp]
Type=0
HeaderFile=Ray.h
ImplementationFile=Ray.cpp
Filter=N

[CLS:CRayDoc]
Type=0
HeaderFile=RayDoc.h
ImplementationFile=RayDoc.cpp
Filter=N

[CLS:CRayView]
Type=0
HeaderFile=RayView.h
ImplementationFile=RayView.cpp
Filter=C
LastObject=ID_FILE_CLOSE
BaseClass=CEditView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_FILE_NEW


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=Ray.cpp
ImplementationFile=Ray.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_RAYTYPE]
Type=1
Class=CRayView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_SCENE
Command18=ID_RECTANGLE
Command19=ID_TRIANGLE
Command20=ID_SPHERE
Command21=ID_ELLIPSOID
Command22=ID_WATER
Command23=ID_HORIZON
Command24=ID_CLOUDS
Command25=ID_MARBLE
Command26=ID_POINTLIGHT
Command27=ID_DIRECTEDLIGHT
Command28=ID_CONELIGHT
Command29=ID_RENDER
Command30=ID_PLAY
Command31=ID_WINDOW_NEW
Command32=ID_WINDOW_CASCADE
Command33=ID_WINDOW_TILE_HORZ
Command34=ID_WINDOW_ARRANGE
Command35=ID_APP_ABOUT
Command36=IDM_MAKROPUTAR
Command37=IDM_CLOUDMACRO
Command38=IDM_TERRAINMACRO
CommandCount=38

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_WIZBAR]
Type=1
Class=?
Command1=ID_SCENE
Command2=ID_RECTANGLE
Command3=ID_TRIANGLE
Command4=ID_SPHERE
Command5=ID_ELLIPSOID
Command6=ID_CYLINDER
Command7=ID_POINTLIGHT
Command8=ID_DIRECTEDLIGHT
Command9=ID_CONELIGHT
Command10=ID_WATER
Command11=ID_HORIZON
Command12=ID_CLOUDS
Command13=ID_MARBLE
Command14=ID_RENDER
Command15=ID_PLAY
CommandCount=15

[CLS:RenderWindow]
Type=0
HeaderFile=RenderWindow.h
ImplementationFile=RenderWindow.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_IMAGE
VirtualFilter=dWC

[DLG:ID_RENDERSCENE]
Type=1
Class=RenderWindow
ControlCount=24
Control1=ID_TRACE,button,1342275584
Control2=ID_IMAGE,static,1342177287
Control3=ID_TIME,edit,1342244992
Control4=IDC_FROM,static,1342308352
Control5=IDC_FROMX,edit,1350631424
Control6=IDC_FROMY,edit,1350631424
Control7=IDC_FROMZ,edit,1350631424
Control8=IDC_AT,static,1342308352
Control9=IDC_ATX,edit,1350631424
Control10=IDC_ATY,edit,1350631424
Control11=IDC_ATZ,edit,1350631424
Control12=IDC_UP,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_UPX,edit,1350631424
Control20=IDC_UPY,edit,1350631424
Control21=IDC_UPZ,edit,1350631424
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352

[DLG:IDD_PLAY]
Type=1
Class=Play
ControlCount=2
Control1=ID_PLAYBACK,button,1342275584
Control2=ID_IMAGEPLAY,edit,1342244993

[CLS:Play]
Type=0
HeaderFile=Play.h
ImplementationFile=Play.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_IMAGEPLAY
VirtualFilter=dWC

[CLS:WindFromX]
Type=0
HeaderFile=WindFromX.h
ImplementationFile=WindFromX.cpp
BaseClass=CEdit
Filter=W

[CLS:WindFromY]
Type=0
HeaderFile=WindFromY.h
ImplementationFile=WindFromY.cpp
BaseClass=CEdit
Filter=W

[CLS:WindFromZ]
Type=0
HeaderFile=WindFromZ.h
ImplementationFile=WindFromZ.cpp
BaseClass=CEdit
Filter=W

[CLS:WindAtX]
Type=0
HeaderFile=WindAtX.h
ImplementationFile=WindAtX.cpp
BaseClass=CEdit
Filter=W

[CLS:WindAtY]
Type=0
HeaderFile=WindAtY.h
ImplementationFile=WindAtY.cpp
BaseClass=CEdit
Filter=W

[CLS:WindAtZ]
Type=0
HeaderFile=WindAtZ.h
ImplementationFile=WindAtZ.cpp
BaseClass=CEdit
Filter=W

