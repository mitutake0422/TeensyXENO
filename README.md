\# TeensyXENO
TeensyXENO用ライブラリ（固定翼機・HILS）

\## Overview

This is a Teensy-based XENO controller library for HILS and fixed-wing applications.

\---

\## 概要

このライブラリは、Teensy上で動作するXENOコントローラ用ライブラリです。

<br>

\## 機能

SBUS入出力
PWM出力
XenoController通信
HILS対応

<br>

\## 動作環境

Teensy 4.0/4.1　　公開時、TeensyXENO基板はTeensy4.0にのみ対応

Arduino IDE

Teensyduino

<br>

\## Teensyduinoインストール方法

1. Arduino IDEを起動
2. ファイル→基本設定
3. 追加のボードマネージャのURLで

　　https://www.pjrc.com/teensy/package_teensy_index.json

　　を入力

5. OKで基本設定ダイアログを閉じる
6. ボードマネージャでTeensyで検索し、Teensy (for Arduino IDE *.*.* or later)をインストール

<br>

\## 使用方法

TeensyXENO基板にTeensy4.0をのせ、PCとはTeensyとはUSBケーブルで接続してください。

<br>

\## サンプルプログラム

examples/TeensyXENO\_HILS を参照してください。
userfunc() 内に誘導制御ロジックを記述します。

<br>

\### TeensyXENOクラスのオブジェクト

TeensyXENO g\_clTX(TEENSY4\_0);

g\_clTXを使って関数呼び出し、メンバ変数へアクセスします。

<br>

\### メンバ変数

\_stSensor\& m\_stSensor;		// センサー値

\_stNavi\& m\_stNavi;		// 航法計算

\_stSBUS\& m\_stSbusRcv;		// SBUS入力

\_stSBUS\& m\_stSbusCmd;		// SBUS出力

\_stDIP\& m\_stDIP;		// DIP SW

\_stFltPlan\& m\_stFltPlan;	// 飛行計画（XenoControllerから送信）

\_stParam\& m\_stParam;		// 誘導制御パラメータ（XenoControllerから送信）

int\& m\_nCtrl;			// 自動操縦ON/OFF

\_stCommand\& m\_stCommand;	// 指令値（出力用）

\_stControl\& m\_stControl;	// 制御計算（出力用）

<br>

\### 定数・構造体定義

src/XenoTypes.h を参照してください。

<br>

\### 記述する場所

user.ino の 関数userfunc

<br>

\### 変数の使い方

例1：加速度X

g\_clTX.m\_stSensor.dAccX

<br>

例2：飛行計画の目標WP

g\_clTX.m\_stFltPlan.nWpTo

<br>

例3：飛行計画のindex = 2のWP緯度

g\_clTX.m\_stFltPlan.stWP\[2].dWpLat

<br>

例4：高度指令（ここに代入するとXenoControllerに表示されます）

g\_clTX.m\_stCommand.dAltCmd

<br>

\---

