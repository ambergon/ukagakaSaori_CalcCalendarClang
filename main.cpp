
#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>

//sqlite
#include <sqlite3.h>

//stringstream
#include <iomanip>

#include <regex>


using namespace std;



FILE* ConsoleWindow;

typedef void* HGLOBAL;
char* resBuf;

char* SaoriDirectory;

int main(int argc, char* argv[]) {
    printf( "%s\n" , argv[0] );
    return 0;
}

#define Debug


//hにはdllまでのLogFilePathが入っている。
//lenはアドレスの長さ。\0の分は入っていない。
extern "C" __declspec(dllexport) bool __cdecl load(HGLOBAL h, long len){

    //loadの長さが不明の時点で下手にfreeするのは危険な気がするな。
#ifdef Debug
    AllocConsole();
    //標準出力(stdout)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stdout);
    //標準エラー出力(stderr)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stderr);
    //文字コードをutf-8に変更。
    //satoriはshift-jis決め打ちである。
    //system( "chcp 65001 > /nul" );
#endif

    SaoriDirectory = (char*)malloc(sizeof(char) * (len + 1 ));
    memset( SaoriDirectory , '\0' , ( len + 1 ));
    memcpy( SaoriDirectory , (char*)h , (int)len  );
    GlobalFree( h );


    printf( "call saori calc calendar load\n" );

    ////パスの日本語対応化
    //char* oldLocale = setlocale( LC_ALL , NULL );
    //setlocale( LC_ALL , "" );
    //dbPATH = (wchar_t*)calloc( len + 1 + strlen( "mailBox.db" ) , sizeof( wchar_t ) );
    //mbstowcs(dbPATH , charDBpath, len + 1 + strlen( "mailBox.db" ));
    //setlocale( LC_ALL , oldLocale );
    //free( charDBpath );


    return true;
}

extern "C" __declspec(dllexport) bool __cdecl unload(void){
    printf( "call saori calc calendar unload\n" );

    free( SaoriDirectory );

#ifdef Debug
    FreeConsole();
#endif

    return true;
}




string ZenToHan( string str ){
    str = regex_replace( str , regex( "�O" ) ,"0" );
    str = regex_replace( str , regex( "�P" ) ,"1" );
    str = regex_replace( str , regex( "�Q" ) ,"2" );
    str = regex_replace( str , regex( "�R" ) ,"3" );
    str = regex_replace( str , regex( "�S" ) ,"4" );
    str = regex_replace( str , regex( "�T" ) ,"5" );
    str = regex_replace( str , regex( "�U" ) ,"6" );
    str = regex_replace( str , regex( "�V" ) ,"7" );
    str = regex_replace( str , regex( "�W" ) ,"8" );
    str = regex_replace( str , regex( "�X" ) ,"9" );
    str = regex_replace( str , regex( "�@" ) ,"" );
    str = regex_replace( str , regex( " " ) ,"" );

    return str;
}

int Checker( string str ){
    str = regex_replace( str , regex( "[0-9]+" ) ,"" );
    str = regex_replace( str , regex( " +" ) ,"" );
    if ( str == "" || str == "-"){
        return 1;
    } else {
        return 0;
    }
}


    //IDは存在しないから書きなぐれる
    //処理について考える。 //置換などに使う文字列はshift-jisで無ければならない。
    //全角数字はいずれも半角に変える。
    //安全チェックとして、半角数字0-9\d+を置換して何もなくなることを確認しよう。
    //問題がある場合はすべて-1を入れよう

extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len){
    char req[*len+1];
    memset( req , '\0' , *len+1 );
    memcpy( req , (char*)h , *len );

//#ifdef Debug
//    printf( "%s----\n" , req );
//#endif

    GlobalFree( h );


    bool NOTIFY = false;
    resBuf = NULL;

    char* Sender    = NULL;


    char* Argument0 = NULL;
    char* Argument1 = NULL;
    char* Argument2 = NULL;
    char* Argument3 = NULL;
    char* Argument4 = NULL;
    char* Argument5 = NULL;
    char* Argument6 = NULL;
    char* Argument7 = NULL;

    char  sepLine[]    = "\r\n";
    char  sepLR[] = ": ";
    char* tp ;
    tp = strtok( req , sepLine );
    while( tp != NULL ){
        if ( tp == "GET Version SAORI/1.0" ){
            tp = strtok( NULL , sepLine );
            continue;
        }

        //左右分割を試みる。
        char* checkR = strstr( tp , sepLR );
        if ( checkR == NULL ){
            tp = strtok( NULL , sepLine );
            continue;
        }


        int Lsize = strlen( tp ) - strlen( checkR ) ;
        char L[ Lsize + 1 ] ;
        memset( L , '\0' , Lsize + 1 );
        memcpy( L , tp , Lsize);

        int Rsize = strlen( tp ) - ( Lsize + strlen( sepLR ) );
        //char R[ Rsize + 1 ] ;
        //memset( R , '\0' , Rsize + 1 );
        //memcpy( R , &tp[ Lsize + strlen( sepLR ) ] , Rsize);

        //printf( "%s\n" , L );
        //printf( "%s\n" , R );
        
        if ( strcmp( L , "Sender" ) == 0 ) {
            Sender = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Sender , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument0" ) == 0 ) {
            Argument0 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument0 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument1" ) == 0 ) {
            Argument1 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument1 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument2" ) == 0 ) {
            Argument2 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument2 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument3" ) == 0 ) {
            Argument3 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument3 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument4" ) == 0 ) {
            Argument4 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument4 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument5" ) == 0 ) {
            Argument5 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument5 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument6" ) == 0 ) {
            Argument6 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument6 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument7" ) == 0 ) {
            Argument7 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument7 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcomp( L , "" ) == 0 ) {
        }
        tp = strtok( NULL , sepLine );
    }



    //引数0 :   年
    //引数1 :   月
    //引数2 :   日
    //引数3 :   追加年
    //引数4 :   追加月
    //引数5 :   追加日
    if ( Argument0 != NULL && Argument1 != NULL && Argument2 != NULL && Argument3 != NULL && Argument4 != NULL && Argument5 != NULL ){
#ifdef Debug
        printf( "before : %s\n" , Argument0 );
        printf( "before : %s\n" , Argument1 );
        printf( "before : %s\n" , Argument2 );
        printf( "before : %s\n" , Argument3 );
        printf( "before : %s\n" , Argument4 );
        printf( "before : %s\n" , Argument5 );
#endif

        string strArgument0 = Argument0;
        string strArgument1 = Argument1;
        string strArgument2 = Argument2;
        string strArgument3 = Argument3;
        string strArgument4 = Argument4;
        string strArgument5 = Argument5;

        //全角半角・スペース処理
        strArgument0 = ZenToHan( strArgument0 );
        strArgument1 = ZenToHan( strArgument1 );
        strArgument2 = ZenToHan( strArgument2 );
        strArgument3 = ZenToHan( strArgument3 );
        strArgument4 = ZenToHan( strArgument4 );
        strArgument5 = ZenToHan( strArgument5 );

        //すべて数字ならば
        if (  Checker( strArgument0 ) && Checker( strArgument1 ) && Checker( strArgument2 ) && Checker( strArgument3 ) && Checker( strArgument4 ) && Checker( strArgument5 ) ) {

            int year        = atoi( strArgument0.c_str() );
            int month       = atoi( strArgument1.c_str() );
            int day         = atoi( strArgument2.c_str() );
            int addyear     = atoi( strArgument3.c_str() );
            int addmonth    = atoi( strArgument4.c_str() );
            int addday      = atoi( strArgument5.c_str() );

            struct tm Base;
            Base.tm_year   = year  - 1900 + addyear     ;
            Base.tm_mon    = month - 1    + addmonth    ;
            Base.tm_mday   = day          + addday      ;
            Base.tm_hour   = 00 ; 
            Base.tm_min    = 00 ; 
            Base.tm_sec    = 00 ; 
            Base.tm_isdst  = 0;

            time_t x = mktime( &Base );
            struct tm Add= *localtime( &x );

            stringstream streamYYYY ;
            streamYYYY << Add.tm_year + 1900 ;
            stringstream streamMM;
            streamMM << Add.tm_mon + 1 ;
            stringstream streamDD;
            streamDD << Add.tm_mday ;

#ifdef Debug
        printf( "after : %s\n" , streamYYYY.str().c_str() );
        printf( "after : %s\n" , streamMM.str().c_str() );
        printf( "after : %s\n" , streamDD.str().c_str() );
#endif

            //返り値
            //nopを使わないで済むようにResultは使わない。
            //"SAORI/1.0 200 OK\r\nCharset: Shift_JIS\r\nResult: " + Res0 + "\r\nValue0: " + ResYear + "\r\nValue1: " + ResMonth + "\r\nValue2: " + ResDay + "\r\n\r\n";
            string Res;
            Res = "SAORI/1.0 200 OK\r\nValue0: " + streamYYYY.str() + "\r\nValue1: " + streamMM.str() + "\r\nValue2: " + streamDD.str() + "\r\n\r\n";

            int i = strlen( Res.c_str() );
            char* res_buf;
            res_buf = (char*)calloc( i + 1 , sizeof(char) );
            memcpy( res_buf , Res.c_str() , i );

            resBuf = res_buf;

            streamYYYY.str("");
            streamYYYY.clear( stringstream::goodbit );

            streamMM.str("");
            streamMM.clear( stringstream::goodbit );

            streamDD.str("");
            streamDD.clear( stringstream::goodbit );
        }
    }






    //Saoriにおいて、Charsetはshift-jis決め打ちである。
    //返すものがなかった時
    if ( resBuf == NULL ){
        //char res_buf[] = "SAORI/1.0 204 No Content\r\nCharset: UTF-8\r\n\r\n";
        char res_buf[] = "SAORI/1.0 204 No Content\r\n\r\n";
        resBuf = res_buf;
    }
    if ( Sender != NULL ){ free( Sender ); }
    if ( Argument0 != NULL ){ free( Argument0 ); }
    if ( Argument1 != NULL ){ free( Argument1 ); }
    if ( Argument2 != NULL ){ free( Argument2 ); }
    if ( Argument3 != NULL ){ free( Argument3 ); }
    if ( Argument4 != NULL ){ free( Argument4 ); }
    if ( Argument5 != NULL ){ free( Argument5 ); }
    if ( Argument6 != NULL ){ free( Argument6 ); }
    if ( Argument7 != NULL ){ free( Argument7 ); }
    
    //pluginは2.0で返す。
    //char res_buf[] = "SAORI/1.0 204 No Content";
    *len = strlen(resBuf);
    HGLOBAL ret = GlobalAlloc(GPTR, (SIZE_T)(*len));
    memcpy(ret, (void*)(resBuf), *len);
    return ret;
}




