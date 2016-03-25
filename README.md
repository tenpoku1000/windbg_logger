# windbg_logger

カーネルデバッグ中の Visual Studio 内蔵 WinDbg の通信内容を記録するアプリケーションとデバイスドライバです。

* Visual Studio 内蔵 WinDbg のすべての通信内容を記録することは保証していません。
* 仮想環境を前提とせず、実機で動作させることを想定しています。
* デバイスドライバをインストールするターゲット PC は、OS が起動しなくなっても問題がないテスト専用の PC を用意してください。

## 開発環境

* Visual Studio Community 2015 + Windows SDK for Windows 10 + Windows Driver Kit 10  
https://msdn.microsoft.com/ja-jp/windows/hardware/hh852365.aspx
* 64 ビット版 Windows 10
* 64 ビット UEFI 対応 PC(ホスト：ノート PC, ターゲット：デスクトップ PC)
* シリアルポートブラケット(D-sub 9 ピン: 対応するマザーボードが必要)
* USB シリアルケーブル(USB type A to D-sub 9 ピン: 通常時 1 本、テスト時 2 本が必要)
* RS-232C クロスケーブル(D-sub 9 ピン)
* USB メモリ

## ビルド方法

* windbg_logger.sln をダブルクリックします。
* Visual Studio のセキュリティ警告を回避してプロジェクトを開きます。  
![warning](https://raw.githubusercontent.com/tenpoku1000/windbg_logger/master/images/MSVC.PNG)
* Visual Studio の起動後に F7 キーを押下します。bin フォルダ以下にアプリケーションとデバイスドライバが生成されます。

## ターゲット PC への KmdfDriver インストール手順

 KmdfDriver のインストール手順については、以下のドキュメントを参照してください。

* [KmdfDriver のインストール手順](/INSTALL.md)

## 実行方法

* ホスト PC に USB シリアルケーブルを接続し、ターゲット PC の COM ポートと RS-232C クロスケーブルで繋ぎます。
* ホスト PC に接続した USB シリアルケーブルの COM ポートを以下のように設定します。  
> COM1, 115200 bps, データビット 8, パリティ無し, ストップビット 1, フロー制御：ハードウェア  

* ホスト PC で windbg_logger\setup\windbg_logger.ini の test_mode を 0 に設定します。
* ホスト PC で windbg_logger\bin\windbg_logger.exe をダブルクリックして起動します。
* ホスト PC で windbg_logger\windbg_logger.sln をダブルクリックします。
* ホスト PC の Visual Studio で KmdfDriver プロジェクトがスタートアッププロジェクトに設定されているのを確認します。
* 以下にブレークポイントが設定されているのを確認します。  
> KmdfDriver プロジェクトの Queue.c ファイルの KmdfDriverEvtIoDeviceControl 関数

* ホスト PC の Visual Studio からターゲット PC の OS にアタッチします(「デバッグ」-「プロセスにアタッチ」を選択)。  
![attach](https://raw.githubusercontent.com/tenpoku1000/windbg_logger/master/images/attach.PNG)
* ホスト PC の windbg_logger\log 以下に WinDbg の電文の記録が開始されます。
* ターゲット PC を再起動します。
* ターゲット PC の OS 起動完了後、ホスト PC の Visual Studio の Debugger Immediate Window に以下の行が表示されていることを確認します。  
> KDTARGET: Refreshing KD connection  

* ホスト PC の Visual Studio で CTRL + Alt + Break 押下、または「デバッグ」-「すべて中断」を選択すると、Debugger Immediate Window に以下のように表示されます。
> Break instruction exception - code 80000003 (first chance)
> *******************************************************************************
> *                                                                             *
> *   You are seeing this message because you pressed either                    *
> *       CTRL+C (if you run console kernel debugger) or,                       *
> *       CTRL+BREAK (if you run GUI kernel debugger),                          *
> *   on your debugger machine's keyboard.                                      *
> *                                                                             *
> *                   THIS IS NOT A BUG OR A SYSTEM CRASH                       *
> *                                                                             *
> * If you did not intend to break into the debugger, press the "g" key, then   *
> * press the "Enter" key now.  This message might immediately reappear.  If it *
> * does, press "g" and "Enter" again.                                          *
> *                                                                             *
> *******************************************************************************
> nt!DbgBreakPointWithStatus:
> fffff801 627bceb0 cc              int     3
> A timeout occurred.  The timeout can be increased in the Debugging options page
>
> kd>

* ホスト PC の Visual Studio の Debugger Immediate Window で g コマンドを入力します。
> kd> g

* ターゲット PC のデスクトップにある C:\bin\Target\UmCmd.exe のショートカットをダブルクリックして実行すると、以下の関数でブレークします。  
> KmdfDriver プロジェクトの Queue.c ファイルの KmdfDriverEvtIoDeviceControl 関数

* ホスト PC の Visual Studio 内蔵の WinDbg でデバッグ操作を行います。
* デバッグ中の KmdfDriverEvtIoDeviceControl 関数から抜ける際に、F5 キーを押下します。
* ホスト PC の Visual Studio を終了させると、windbg_logger.exe も終了します。
* ターゲット PC の UmCmd.exe の画面で Enter キーを押すと UmCmd.exe が終了します。

## ターゲット PC の KmdfDriver のアンインストール手順

* ターゲット PC で管理者権限で cmd.exe を起動し、以下のコマンドを入力します。  
> cd \bin  
> devcon /r remove Root\KmdfDriver  

* cmd.exe の画面に以下のように表示されたら、KmdfDriver のアンインストールは終了です。  
> ROOT\SAMPLE\0000                                            : Removed  
> 1 device(s) were removed.  

## ターゲット PC の設定を元に戻す手順

* ターゲット PC で管理者権限で cmd.exe を起動し、以下のコマンドを入力します。  
> bcdedit /debug off  
> bcdedit /set TESTSIGNING OFF  

* ターゲット PC を再起動します。
* UEFI セキュアブート対応 PC の場合、必要なら UEFI セキュアブートを有効化します。
* OS 起動後、デバイスマネージャで COM1 が表示されます。

## ホスト PC 単独でテストを行う方法

* ホスト PC に USB シリアルケーブル 2 本を接続し、双方を RS-232C クロスケーブルで繋ぎます。
* ホスト PC に接続した USB シリアルケーブル 2 本を以下のように設定します。  
> COM1, 115200 bps, データビット 8, パリティ無し, ストップビット 1, フロー制御：ハードウェア  
> COM2, 115200 bps, データビット 8, パリティ無し, ストップビット 1, フロー制御：ハードウェア  

* ホスト PC で windbg_logger\setup\windbg_logger.ini の test_mode を 1 に設定します。
* ホスト PC で windbg_logger\bin\windbg_logger.exe をダブルクリックして起動します。
* ホスト PC で windbg_logger\bin\test_client.exe をダブルクリックして起動します。
* windbg_logger\log 以下に WinDbg のテスト電文が記録されます。
* test_client.exe の画面に SUCCESS: To exit, please press enter key. と表示されたら、Enter キーを押すとテストは終了です。
* windbg_logger.exe は自動で終了します。

## 謝辞

本ソフトウェアは、以下の外部プロジェクトの成果物を利用しています。感謝いたします。
元のソフトウエアからの変更点は、一部の include ファイル(windbgkd.h 等)のみ利用している点です。

### GPLv2 or later
* ReactOS https://www.reactos.org/ja

## ライセンス

[GPLv3](https://raw.githubusercontent.com/tenpoku1000/windbg_logger/master/COPYING3)

## 作者

市川 真一 <suigun1000@gmail.com>

## 参考資料

* Debugger Commands - Windows 10 hardware dev  
https://msdn.microsoft.com/en-us/library/windows/hardware/ff540507(v=vs.85).aspx
* Kernel and remote debuggers - tutorial - developer Fusion  
http://www.developerfusion.com/article/84367/kernel-and-remote-debuggers/
* KD Extension DLLs &amp; KDCOM Protocol  
http://articles.sysprogs.org/kdvmware/kdcom/
* j00ru//vx tech blog : Attacking the Host via Remote Kernel Debugger (Virtual Machines)  
http://j00ru.vexillium.org/?p=405
* Just Another Windows Kernel Perl Hacker  
https://www.blackhat.com/presentations/bh-usa-07/Stewart/Presentation/bh-usa-07-stewart.pdf
* ChangeLog-0.3.11 - ReactOS Wiki  
https://www.reactos.org/wiki/ChangeLog-0.3.11#KD
* アプリケーションからのデバイスドライバの呼び出し、制御方法について  
https://social.msdn.microsoft.com/Forums/windows/ja-JP/6ac17ef7-2c9f-4486-8f75-126fe20d2d50?forum=windowsgeneraldevelopmentissuesja
* 管理者権限でしかアクセスできません | WDDDC  
http://www.wdddc.net/node/143
* IoCreateDeviceSecure problem | Windows Vista Tips  
http://www.winvistatips.com/threads/iocreatedevicesecure-problem.180268/
* 0と1の欠片 : [Visual Studio]ドライバをビルドすると、&quot;22.9.7: DriverVer set to a date in the future (postdated DriverVer not allowed) in \xxxx.inf.&quot;というエラーになる  
http://blog.livedoor.jp/cielo_cielo/archives/65844807.html
