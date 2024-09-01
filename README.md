# ELSynchronizer

<img src="doc\image\medium.jpeg" alt="medium.jpeg" width="400px">

## 概要

$いーえるしんくろないざー \atop{\large \rm{ELSynchronizer}}$は、複数台のELの動作を同期するためのデバイスです。

リズムスタート/ストップの完全な同期により、以下のようなことが可能です。

- カウントの同期による無音のカウント出しやテンポキープ
- オートレジストの同期
- 2つ以上のドラムキットの使用

## 機能

1台のELから接続している全てのELに同一のMIDIメッセージを送信します。

これにより全てのELのSEQボタンとSTART/STOPボタンを1台から制御することができます。

![overview.png](doc\image\overview.png)

以下、制御するELを「親EL」、制御されるELを「子EL」と呼びます。

親ELからは種々のMIDIメッセージが出力されており、演奏に不都合となるメッセージも含まれています。ELSynchronizerでは内部処理により、以下のメッセージのみ子ELへ出力します。

- SEQ 1~4のON/OFF
- リズムのSTART/STOP

## 使い方

### 各部の名称

![parts_name.png](doc\image\parts_name.png)

### 必要なもの

- USBケーブル(USB Type-B ↔ USB Type-A など)：1本
- ACアダプタ(USBケーブルが使えればなんでも)：1個
- MIDIケーブル：ELの台数分

### セットアップ

1. 親ELのMIDI OUT端子と①[MIDI IN]をMIDIケーブルで接続する
2. 子ELのMIDI IN 端子と③~⑥[MIDI OUT]をMIDIケーブルで接続する
3. ⑦[電源]をUSBケーブルで電源に接続する
    1. コンセントからとる場合はACアダプター等を使用する
    2. ELからとる場合はUSB Type-A端子[TO DEVICE]を使用する(非推奨)
4. ELSynchronizerのLEDが光れば起動成功

![connection.png](doc\image\connection.png)

### 演奏での使用方法

親ELでSEQ1~4ボタン・STARTボタンを押下すると、子ELでも同じ動作をします。

MIDIケーブルで接続したELは全て強制的に同期されるため、同期したくないELからはMIDIケーブルを抜いてください。

※同期するELを物理ボタンで選択する機能を開発予定です。

## 使用例

### 複数台のELで同時にリズムスタートする

親ELでSEQボタンやSTARTボタンを押すことで、子ELでも同時にリズムをスタートすることができます。

### 2つ以上のドラムキットを使用する

親ELおよび子ELのSEQにそれぞれ異なるリズムパターンを組み、これらを同時再生することで実質的に2つ以上のドラムキットを使用することができます。例えばELを4台使う場合、(main drum + add drum) * 4で合計8つまで使用することができます。

### 複数のELでオートレジストを使用する

同期させる全てELのSEQにオートレジストを組み同時に再生することでズレのないオートレジストが実現できます。

### ドラムがない曲でテンポキープに使用する

無音のリズムを同期して再生することで、各ELで同一のカウントを見ることができます。クラシックや曲中のブレイクなどドラムの音がなくテンポキープが難しい場合に、サポートとして使用することができます。

### カウント出しをせず演奏を合わせる

無音のリズムを同期して再生することで、各ELで同一のカウントを見て演奏を始めることが出来ます。

## 注意事項

- このデバイスは構造上**非常にもろい**です。扱いには十分注意してください。
- ELSynchronizer本体の端子の頻繁な抜き差しは避け、EL側の端子を抜き差しすることで対応してください。

## 技術的仕様

### 構成

ELSynchronizerは3種類のパーツから構成されています。各パーツは組み替えできるようにArduino Unoのシールドとして作製しています。

#### MIDIシールド

SparkFun製のMIDIシールドです。単にMIDI IN/OUTとして使っています。このデバイスの中で最も高価なパーツなので壊さないように。

<img src="doc\image\midi_sheild.png" alt="midi_sheild.png" width="400px">

#### MIDI OUT用シールド

2口のMIDI OUT端子をつけた自作シールドです。2つ作製しています。使用するELの台数に応じてMIDI OUT端子を増やすことができます。すべてGPIOの1pin(TX)を使用しています。

<img src="doc\image\midi_out_sheild.png" alt="midi_out_sheild.png" width="400px">

#### Arduino Uno R3

(互換品です。)内部処理に使用しています。ELSynchronizerは設計的にArduino Uno R4 Minimaとの互換性がないようので注意(原因不明)。

<img src="doc\image\arduino_uno_r3.png" alt="arduino_uno_r3.png" width="400px">

### 組み替え機能

使用するMIDI OUT端子の数に応じてデバイスの組み替えが可能です。サイズを小さくして使用したいときなどに有用です。

#### MIDI IN×1, MIDI OUT×1のすがた

<img src="doc\image\small.jpeg" alt="small.jpeg" width="400px">

#### MIDI IN×1, MIDI OUT×3のすがた

<img src="doc\image\medium.jpeg" alt="medium.jpeg" width="400px">

#### MIDI IN×1, MIDI OUT×5のすがた

<img src="doc\image\big.jpeg" alt="big.jpeg" width="400px">

## 今後実装予定の機能

- [ ] ケースの作製
- [ ] 物理ボタンによる各ELの同期/非同期選択機能
- [ ] Exp.の同期/非同期の切り替え機能
- [ ] PCとの同期