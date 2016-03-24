## ターゲット PC への KmdfDriver インストール手順

# ホスト PC でインストール用ファイルの USB メモリへのコピー

* windbg_logger\tools\cp.cmd をダブルクリックします。
* windbg_logger\tools\bin 以下で、以下のファイルの存在を確認します。  
> INSTALL.md  
> README.md  
> devcon.exe  
> Target\UmCmd.exe  
> Target\KmdfDriver\KmdfDriver.inf  
> Target\KmdfDriver\KmdfDriver.sys  
> Target\KmdfDriver\KmdfDriver.cat  

* USB メモリをフォーマットします。
* windbg_logger\tools\bin フォルダを USB メモリにコピーします。

# ターゲット PC の準備作業

* UEFI セキュアブート対応 PC の場合、UEFI セキュアブートを無効化します。
* USB メモリの bin フォルダをターゲット PC の C:\ にコピーします。
* ターゲット PC で管理者権限で cmd.exe を起動し、以下のコマンドを入力します。  
> bcdedit /debug on  
> bcdedit /dbgsettings SERIAL DEBUGPORT:1 BAUDRATE:115200  
> bcdedit /set TESTSIGNING ON  

* ターゲット PC を再起動します。
* ターゲット PC の OS 起動後、デスクトップの右下に以下のように表示されます。  
![test_mode](https://raw.githubusercontent.com/tenpoku1000/windbg_logger/master/images/test_mode.PNG)
* デバイスマネージャで COM1 が表示されなくなります(Windows カーネルで占有するため)。

# ターゲット PC への KmdfDriver のインストール

* ターゲット PC で管理者権限で cmd.exe を起動し、以下のコマンドを入力します。  
cd \bin  
devcon /r install C:\bin\Target\KmdfDriver\KmdfDriver.inf Root\KmdfDriver  
* 以下の画面が表示されます。「このドライバーソフトウェアをインストールします」を選択します。  
![install](https://raw.githubusercontent.com/tenpoku1000/windbg_logger/master/images/install.PNG)
* cmd.exe の画面に以下のように表示されたら、KmdfDriver のインストールは成功です。  
> Device node created. Install is complete when drivers are installed...  
> Updating drivers for Root\KmdfDriver from C:\bin\Target\KmdfDriver\KmdfDriver.inf.  
> Drivers installed successfully.  

* デバイスマネージャで WdbgLog\KmdfDriver Device が表示されます。  
![devmng](https://raw.githubusercontent.com/tenpoku1000/windbg_logger/master/images/devmng.PNG)
