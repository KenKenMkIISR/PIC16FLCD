# 8ピン8bit PICマイコンによるカラー液晶利用実験
8ピンの8bitマイコンPIC16F18313でカラーグラフィック液晶を利用する実験をしました。  
実験題材として単純なブロック崩しゲームを作成しました。  
電源として+3.3Vが必要です。  
  
## 接続
PIC16F18313   - ILI9341(SPI)  
1 VDD +3.3V  
2 RA5(LEFT button)  
3 RA4         - RESET  
4 MCLR(RA3/RIGHT button)  
5 RA2         - DC  
6 RA1(ICSPCLK)- SCK  
7 RA0(ICSBDAT)- SDI(MOSI)  
8 VSS (GND)  
                CS -> GND  
                SDO(MISO) -> (NC)  
  
## 遊び方
ファイルbreakout.hexをPICkitなどでPICマイコンに書き込んだ後、電源オンで起動します。  
いずれかのボタンを押すとゲームがスタートします。
失敗しても特にゲームオーバーにはなりません。
全部のブロックを消すと最初に戻り、またいずれかのボタンでゲームスタートします。  
衝突判定が少し甘いところや、ブロックの隅が微妙に欠けたりすることがあります。
あくまで液晶利用の実験だと思ってご容赦ください。  
ソースファイルを編集したい場合は、MPLAB XとXC8コンパイラでビルドしてください。
プログラム作成にはMicrochip Code Configulatorを利用しました。ビルドには不要です。  
  
![](breakout.jpg)  

