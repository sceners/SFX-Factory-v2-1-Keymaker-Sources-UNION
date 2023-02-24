///////////////////////////////////////////////////////////////////////////
//
// SFXFactory v2.1 for Windows 9x/NT Keymaker Open Source Code
// Written by Egis/CORE/UNiON.
//
///////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include "resource.h"
#include "bn.h"
#include "crc.h"

HINSTANCE	hInst;
HWND		hWnd;

///////////////////////////////////////////////////////////////////////////
void UUEnCode( BN *N, BYTE *Key )
{
	char	Base[] = { "34679ACDEFHKLMNPQRSTWXY" }, i;

	for( i = 0; i < 0x10; i ++ )
		Key[ i ] = Base[ ( BYTE ) BN_sDiv( N, ( BN ) 0x17 ) ];
	Key[ 0x10 ] = '\0';
}

///////////////////////////////////////////////////////////////////////////
void GenKey( char* buf, BYTE* Key )
{
	static BYTE SFX_N[] = { 0x15, 0x60, 0xd6, 0x72, 0x3e, 0x06, 0xb1, 0x98, 0x9b, 0x00, 0x00, 0x00 };
	static BYTE SFX_d[] = { 0x73, 0x27, 0x20, 0x92, 0xAE, 0xAE, 0x20, 0xBB, 0x67, 0x00, 0x00, 0x00 };
	static BYTE t[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	BYTE	bKey[ 0x11 ];

	BN_ExpMod( ( BN * )t, ( BN * )Key, ( BN * )SFX_d, ( BN * )SFX_N );
	UUEnCode( ( BN * )t, bKey );
	wsprintf( buf, "ACE%s", bKey );
}

///////////////////////////////////////////////////////////////////////////
void Generate( void )
{
	BYTE	szName[ 80 ], szCode[ 50 ];
	BYTE	Key[ 12 ] = { 0 };
	DWORD	dwCRC;

	if( !GetDlgItemText( hWnd, IDC_NAMEEDIT, ( char* ) szName, 80 ) ) {
		MessageBox( hWnd, "Invalid name.", "Error", MB_OK | MB_ICONSTOP );
		return;
	}

	strupr( ( char* ) szName );
	dwCRC = GenCRC( szName, strlen( ( char* ) szName ) );
	*( ( DWORD * ) Key ) = dwCRC;
	Key[ 7 ] = 0xc3;
	Key[ 6 ] = 6;
	GenKey( ( char* ) szCode, Key );
	SetDlgItemText( hWnd, IDC_CODEEDIT, ( char* ) szCode );
}

///////////////////////////////////////////////////////////////////////////
LONG APIENTRY SFXKeyDlg( HWND hDlg, UINT message, UINT wParam, LONG lParam )
{
	switch( message ) {
		case WM_SHOWWINDOW:
			SetWindowText( hDlg, "SFX Factory v2.1 Keymaker by Egis/CORE" );
			SetFocus( GetDlgItem( hDlg, IDC_NAMEEDIT ) );
			break;

		case WM_COMMAND:
			if( LOWORD( wParam ) == IDC_GENBUTTON ) {
				Generate();
				return TRUE;
			}
			break;

		case WM_DESTROY:	
			PostQuitMessage( 0 );
			return TRUE;
	}
	return DefWindowProc( hDlg, message, wParam, lParam );
}

///////////////////////////////////////////////////////////////////////////
BOOL InitApplication( HINSTANCE hInstance )
{
	WNDCLASS  wc;
	
	wc.style			= CS_HREDRAW + CS_VREDRAW;
	wc.lpfnWndProc		= ( WNDPROC ) SFXKeyDlg; 
	wc.cbClsExtra		= 0;              
	wc.cbWndExtra		= DLGWINDOWEXTRA;
	wc.hInstance		= hInst;       
	wc.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= ( HBRUSH ) COLOR_WINDOW; 
	wc.lpszMenuName		= NULL;  
	wc.lpszClassName	= "Key";
	
	return RegisterClass( &wc );
}


///////////////////////////////////////////////////////////////////////////
HWND InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND hWndMain;

	hWndMain = CreateDialogParam( hInstance,
		MAKEINTRESOURCE( IDD_REGDIALOG ), 0, NULL, 0 );
	
	if( !hWndMain )
		return NULL;
	
	return hWndMain;      
}


///////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,	LPSTR lpCmdLine, int nCmdShow )
{
	MSG 	msg;      

	hInst = hInstance;
	if( !InitApplication( hInstance ) )
		return FALSE;   

	if( !( hWnd = InitInstance( hInstance, nCmdShow ) ) )
		return FALSE;

	while( GetMessage( &msg, NULL, 0, 0 ) ) {
		if( !IsDialogMessage( hWnd, &msg ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg ); 
		}
	}
	return msg.wParam;			
}
