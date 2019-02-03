# LINE Things で実装してみた
この内容は、[LINE Things Developer Trial ハンズオン](https://qiita.com/hktechno/items/bb83cc898c75819b2664)をもとに実装してみた際の感想、注意点を主にまとめている。
- ## 目次
    - LINE-things とは
    - LINE-things の特徴
    - LINE-things の実装法
- ## LINE-things とは
    - LINE-things とは、LINE アプリから**Blooth LE (以下 BLE)対応デバイス**を直接操作できるLINEサービスです。
    - 2019年前半には、LINE アプリを起動させなくてもLINE Things が使用できる**Automated BLE communication** が実装されます。
    
- ## LINE-things の特徴
    - 開発可能なデバイスの条件には、
    LINE Things対応デバイスは、以下の条件を満たす必要があります。
        >- BLE 4.2以上に対応している
        >- プロダクトを識別するために、プロダクト独自のGATT Service UUID（※）がある
        >- ボンディングの前に、プロダクト独自のGATT Service UUID（※）をアドバタイズできる
        >- ボンディングができる
        >- ボンディング後の通信が暗号化される
        >- デバイス特定用characteristicから、プロダクト単位でデバイスを一意に識別する値を読み込める
        >-デバイス特定用characteristicは、属性のパーミッションを暗号化（Encryption）に設定してください。LIFFアプリからLINE Things対応デバイスにアクセスする際は、認証済み鍵によって暗号化された接続でアクセスします。*(https://developers.line.biz/ja/docs/line-things/create-product/ より引用)*

    - 基本的には、市販のESP32,obniz などであれば、問題ないそうですが、**raspberry pi**(以下、ラズパイ)では、bluetooth ではうまく作動しないそうなので、ラズパイだけでの開発は諦めたほうがよさそうです。
    - [LINE Things を使用したデバイス例](https://qiita.com/hktechno/items/bb83cc898c75819b2664#line-things-%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%9F%E3%83%87%E3%83%90%E3%82%A4%E3%82%B9%E3%81%AE%E4%BD%9C%E4%BE%8B)では、Arduinoで実装していましたが、BLE の開発ができれば、どの言語でも可能です。

- ## LINE thingsの実装上の注意点
    - LINE ThingsでUUIDの取得する際、curl コマンドを使って、LINE API のサーバーに データを送信しなくてはならない。しかし,コマンドラインから送信してもがなかなかうまくいかない場合がある。その際は、**shell(windows なら batch) やphp などでファイルをあらかじめ作成したのち、POSTした方がいい**。
    - LINE things では、POST通信で登録できるデバイスは限りがあります。そのため、もし、登録したUUIDの確認、更新、削除場合は、[LINEの公式リファレンス](https://developers.line.biz/ja/reference/line-things/#get-product-id-and-psdi)を参考に、APIに通信してください。
    - このリポジトリには、上記のGET,POSTのをより手軽にするためのpython ファイルを用意した。get.py はGET通信、post.py はPOST通信を行うものである。それぞれのファイルに、"channel_token","name","liffId" を記述し、同じディレクトリにて、コマンドラインより<br>
    `python <使用したい通信>.py`<br>と送信してください。**（ただし、あらかじめ、"requests"モジュールが使用可能な状態にしてください。）**