#ifndef __WEB_INTERFACE_404_HTML_H
#define __WEB_INTERFACE_404_HTML_H

#if defined(__ICCARM__)
#pragma location = \
    .qspi static const char not_found_html[] = {
#else
__attribute__((section(".qspi"))) static const char not_found_html[]= {
#endif

"HTTP/1.0 200 OK\r\n"
"Server: lwIP/pre-0.6\r\n"
"Content-type: text/html\r\n"
"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
"<html>\n"
"\n"
"<head>\n"
"    <title>404</title>\n"
"</head>\n"
"<body style=\"color: black; background-color: white;\">\n"
"    <table width=\"100%\">\n"
"        <tbody>\n"
"            <tr valign=\"top\">\n"
"                <td width=\"500\">\n"
"                    <h1>404 - Page not found</h1>\n"
"                </td>\n"
"            </tr>\n"
"        </tbody>\n"
"    </table>\n"
"</body>\n"
"</html>\n"
};

#endif
