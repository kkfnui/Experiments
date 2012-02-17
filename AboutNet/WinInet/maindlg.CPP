#include "StdAfx.h"
#include "MainDlg.h"

static PCWSTR URL_PROTOCOL_HTTP = L"http://";
static PCWSTR URL_PROTOCOL_FTP = L"ftp://";
static PCWSTR URL_PROTOCOL_ED2K = L"ed2k://";
static PCWSTR URL_PROTOCOL_THUNDER = L"thunder://";
static PCWSTR URL_PROTOCOL_MAGNET = L"magnet:?";
static PCWSTR URL_PROTOCOL_RTSP = L"rtsp://";
static PCWSTR URL_PROTOCOL_RTSPT = L"rtspt://";
static PCWSTR URL_PROTOCOL_MMS = L"mms://";
static PCWSTR URL_PROTOCOL_MMST = L"mmst://";
static PCWSTR URL_PROTOCOL_HTTPS = L"https://";
static PCWSTR URL_PROTOCOL_FTPS = L"ftps://";

LRESULT CMainDlg::OnBnClickedAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CWindow wndUrl = GetDlgItem(IDC_EDIT_URL);
    wndUrl.GetWindowText(strUrl);
    BOOL bDone = FALSE;
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;

    do 
    {
        std::wstring strUserAgentW= _T("Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
        hSession = ::InternetOpen(strUserAgentW.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (hSession)
        {

            TCHAR szDomain[MAX_PATH];
            INTERNET_PORT nServerPort = 0;
            if (!GetDomainAndPortFromUrl(strUrl, szDomain, nServerPort))
            {
                bDone = FALSE;
                break;
            }

            hConnect = InternetConnect(hSession,
                szDomain,
                nServerPort,
                NULL,
                NULL,
                INTERNET_SERVICE_HTTP,
                0,
                0);
            if (hConnect)
            {
                DWORD dwOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |  
                    INTERNET_FLAG_KEEP_CONNECTION |  
                    INTERNET_FLAG_NO_AUTH |  
                    INTERNET_FLAG_NO_UI |
                    INTERNET_FLAG_NO_COOKIES |
                    INTERNET_FLAG_IGNORE_CERT_CN_INVALID|
                    INTERNET_FLAG_IGNORE_CERT_DATE_INVALID; 
                if (nServerPort == INTERNET_DEFAULT_HTTPS_PORT)
                {
                    dwOpenRequestFlags |= INTERNET_FLAG_SECURE;
                }


                int nPos = strUrl.Find(szDomain, 0);
                strUrl = strUrl.Right(strUrl.GetLength() - nPos - wcslen(szDomain));

                hRequest = HttpOpenRequest(hConnect, NULL, strUrl, NULL,  
                    NULL, NULL,  
                    dwOpenRequestFlags, 0);
                LPCTSTR strContentType = L"Accept-Language:zh-CN";
                CString strHeader(strContentType);
                //strHeader.Format(strContentType, m_strCookies); 
                HttpAddRequestHeaders(hRequest,
                    strHeader,
                    strHeader.GetLength(),
                    HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);  

                if (hRequest)
                {
                    BOOL bResult = HttpSendRequest(hRequest, NULL, 0, NULL, 0);  
                    if ( ! bResult && GetLastError() == ERROR_INTERNET_INVALID_CA )   
                    {  
                        DWORD dwFlags;  
                        DWORD dwBuffLen = sizeof(dwFlags);  
                        InternetQueryOption (hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwBuffLen);  
                        dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;  
                        InternetSetOption (hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof (dwFlags) ); 
                        bResult = HttpSendRequest(hRequest, NULL, 0, NULL, 0); 
                    }
                    if (bResult)
                    {
                        DWORD dwRetCode = 0;
                        WCHAR szRetCode[64] = {0};
                        DWORD dwSize = 64;
                        DWORD dwIndex = 0;
                        ::HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE ,&szRetCode, &dwSize, &dwIndex);
                        dwRetCode = _tstol(szRetCode);
                        if (dwRetCode < HTTP_STATUS_BAD_REQUEST )
                        {
                            //获取文件名称
                            TCHAR szContentDisposition[512] = {0};
                            DWORD dwContentDispositionSize = 512;
                            dwIndex = 0;
                            if (::HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_DISPOSITION, szContentDisposition, &dwContentDispositionSize, &dwIndex))
                            {
                                //attachment; filename="a_log.zip"
                                CString strDisposistion = szContentDisposition;
                                GetDlgItem(IDC_EDIT_FILENAME).SetWindowText(strDisposistion);   
                                bDone = TRUE;
                            }
                        }

                        else
                        {
                            bDone = FALSE;
                            break;
                        }
                    }
                }
                else
                {
                    bDone =FALSE;
                    break;
                }
            }
            else
            {
                bDone = FALSE;
                break;
            }
        }
    } while (0);

    if (hRequest)
    {
        ::InternetCloseHandle(hRequest);
    }
    if (hConnect)
    {
        ::InternetCloseHandle(hConnect);
    }
    if (hSession)
    {
        ::InternetCloseHandle(hSession);
    }

    return 0;
}


BOOL CMainDlg::GetDomainAndPortFromUrl(LPCTSTR strUrl, LPTSTR strDomain, INTERNET_PORT &nServerPort)
{
    ATLASSERT(strUrl);
    const int PORT_MAX_LENGTH = 5;
    TCHAR szPort[PORT_MAX_LENGTH];
    DWORD dwPort = PORT_MAX_LENGTH;
    HRESULT hPort = UrlGetPart(strUrl, 
        szPort, 
        &dwPort, 
        URL_PART_PORT, 
        NULL);
    if (hPort != S_OK)
    {
        nServerPort  = INTERNET_DEFAULT_HTTP_PORT;
        if( wcsnicmp(strUrl, URL_PROTOCOL_HTTPS, wcslen(URL_PROTOCOL_HTTPS)) == 0)
        {
            nServerPort  = INTERNET_DEFAULT_HTTPS_PORT;
        }
    }
    else
    {
        nServerPort = static_cast<INTERNET_PORT>(_wtol(szPort));
        ATLASSERT(nServerPort != 0);
        if (nServerPort == 0)
        {
            nServerPort = INTERNET_DEFAULT_HTTP_PORT;
        }
    }
    DWORD dwlen = static_cast<DWORD>(wcslen(strDomain));
    HRESULT hDomain = UrlGetPart(strUrl, 
        strDomain, 
        &dwlen, 
        URL_PART_HOSTNAME, 
        NULL);

    if (hDomain != S_OK)
    {
        return FALSE;
    }
    return TRUE;
}