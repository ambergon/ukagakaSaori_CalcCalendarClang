# ukagakaSaori_CalcCalendar/Clang
以前C#で書いたものには、SAORIの使用後に、ゴーストのアンインストール等で、SAORIが残ってしまう問題があったのでCで書き直しました。
前回 : [GitHub - ambergon/ukagakaSaori_CalcCalendar](https://github.com/ambergon/ukagakaSaori_CalcCalendar)


## 導入
`saori_conf.txt`に下記を追記
```
＠SAORI
＃呼び出し名,saori/xxx.dll
CalendarAdd,Saori/ukagakaSaori_CalcCalendar.dll
```


## 使い方
前回のものと一部変更されました。
全角数字を半角数字に変換するため、事前にZen2Hanを撃つ必要がなくなりました。
これはShift-JIS環境のみ変換されます。(SATORIユーザは気にしないでよい。)
また、Resultを返さないため、nopで返り値を処分する必要がなくなりました。

それぞれのS0...は半角数字で返ってきます。
```
＊0headつつかれ
：計算
＃（CalendarAdd,（現在年）,（現在月）,（現在日） ,追加年,追加月,追加日）
（CalendarAdd,（現在年）,（現在月）,（現在日） ,０,－２,１）
現在年 : （現在年）
現在月 : （現在月）
現在日 : （現在日）
計算年 : （S0）
計算月 : （S1）
計算日 : （S2）
```

## License
このプログラムを使ったいかなる問題や損害に対して、私は責任を負いません。
また、ゴーストなどに同梱して配布していただいて構いません。


## Author
ambergon
