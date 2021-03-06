; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "flashmcu"
#define MyAppVersion "1.0.0.2"
#define MyAppPublisher "BMSTU"
#define MyAppURL "https://github.com/pi-null-mezon/Lighthelmet"
#define MyAppExeName "flashmcu.exe"

#define MyAppArch "x86"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{F839BF4F-754F-4414-927C-4311863719F4}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultGroupName={#MyAppPublisher}\{#MyAppName}
DefaultDirName={pf}\{#MyAppName}
OutputDir=C:\Programming\Releases
OutputBaseFilename=SETUP_{#MyAppName}_v{#MyAppVersion}_gcc_{#MyAppArch}
Compression=lzma
SolidCompression=yes

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Programming\Lighthelmet\build\build-flashmcu-Desktop_Qt_5_9_1_MinGW_32bit-Release\release\flashmcu.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\Qt5Concurrent.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\Qt5SerialPort.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\bin\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\plugins\platforms\qminimal.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\plugins\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "C:\Qt\5.9.1\mingw53_32\plugins\imageformats\qsvg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
;Add CH340G USB-UART driver
Source: "C:\Programming\3rdParties\Devices\CH341SER\*"; DestDir: "{app}\CH341SER"; Flags: ignoreversion recursesubdirs; Check: InstallUSBUARTDriver
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
;Add CH340G USB-UART driver install 
Filename: "{app}\CH341SER\SETUP.exe"; Flags: waituntilterminated; Check: InstallUSBUARTDriver
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
//-------------------------------------------------------------------------------------
// Delete app if it had been installed before
//-------------------------------------------------------------------------------------
function GetUninstallString: string;
var
  sUnInstPath: string;
  sUnInstallString: String;
begin
  Result := '';
  sUnInstPath := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\{{F839BF4F-754F-4414-927C-4311863719F4}_is1'); //Your App GUID/ID
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;

function IsUpgrade: Boolean;
begin
  Result := (GetUninstallString() <> '');
end;

function InitializeSetup: Boolean;
var
  V: Integer;
  iResultCode: Integer;
  sUnInstallString: string;
begin
  Result := True; // in case when no previous version is found
  if RegValueExists(HKEY_LOCAL_MACHINE,'Software\Microsoft\Windows\CurrentVersion\Uninstall\{F839BF4F-754F-4414-927C-4311863719F4}_is1', 'UninstallString') then  //Your App GUID/ID
  begin
    V := MsgBox(ExpandConstant('An old version of app has been detected. Uninstall now?'), mbInformation, MB_YESNO); //Custom Message if App installed
    if V = IDYES then
    begin
      sUnInstallString := GetUninstallString();
      sUnInstallString :=  RemoveQuotes(sUnInstallString);
      Exec(ExpandConstant(sUnInstallString), '', '', SW_SHOW, ewWaitUntilTerminated, iResultCode);
      Result := True; //if you want to proceed after uninstall
      //Exit; //if you want to quit after uninstall
    end
    else
      Result := False; //when older version present and not uninstalled
  end;
end;
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Install CH340G driver?
//-------------------------------------------------------------------------------------
var
  USBUARTDRVPage: TInputOptionWizardPage;  

procedure InitializeWizard;
begin
  USBUARTDRVPage := CreateInputOptionPage(wpWelcome,
    'Installation option', 'Do you want to also install CH340G USB-UARD driver?',
    'Please specify option',
    True, False);
  USBUARTDRVPage.Add('Yes (install USB-UART driver)');
  USBUARTDRVPage.Add('No (do not install USB-UART driver)');
  USBUARTDRVPage.SelectedValueIndex := 0;
end;

function InstallUSBUARTDriver: Boolean;
begin
  // here is the Check function used above; if you return True to this
  // function, the file will be installed, when False, the file won't 
  // be installed
  case USBUARTDRVPage.SelectedValueIndex of
    0: Result := True;
    1: Result := False;
  end;
end;