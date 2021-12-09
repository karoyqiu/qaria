/*! ***********************************************************************************************
 *
 * \file        C:/Users/karoy/source/repos/qaria2/tools/../src/aria2optionsbuilder.h
 * \brief       OptionsBuilder 类头文件。
 * \version     0.1
 * \date        2021-12-08
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2021 karoyqiu。
 *
 **************************************************************************************************/
#pragma once
#include <QVariantHash>

inline constexpr qint64 operator"" _K(quint64 value)
{
    return value * 1024;
}

inline constexpr qint64 operator"" _M(quint64 value)
{
    return value * 1024 * 1024;
}

inline constexpr qint64 operator"" _min(quint64 value)
{
    return value * 60;
}

inline constexpr qint64 operator"" _h(quint64 value)
{
    return value * 60 * 60;
}

inline constexpr qint64 operator"" _d(quint64 value)
{
    return value * 60 * 60 * 24;
}


class OptionsBuilder
{
public:
    //////////////////////////////////////////////////////////////////////////
    //
    // Basic Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * The directory to store the downloaded file.
     */
    void setDir(const QString &dir)
    { hash_.insert(QS("dir"), dir); }

    /*!
     * Downloads the URIs listed in ``FILE``. You can specify multiple sources for a
     * single entity by putting multiple URIs on a single line separated by the
     * :kbd:`TAB` character.
     * Additionally, options can be specified after each URI line. Option lines
     * must start with one or more white space characters (:kbd:`SPACE` or :kbd:`TAB`)
     * and must only contain one option per line.
     * Input files can use gzip compression.
     * When ``FILE`` is specified as ``-``, aria2 will read the input from ``stdin``.
     * See the `Input File`_ subsection for details.
     * See also the :option:`--deferred-input` option.
     * See also the :option:`--save-session` option.
     */
    void setInputFile(const QString &file)
    { hash_.insert(QS("input-file"), file); }

    /*!
     * The file name of the log file. If ``-`` is specified, log is written to
     * ``stdout``. If empty string("") is specified, or this option is omitted,
     * no log is written to disk at all.
     */
    void setLog(const QString &log)
    { hash_.insert(QS("log"), log); }

    /*!
     * Set the maximum number of parallel downloads for every queue item.
     * See also the :option:`--split <-s>` option.
     * Default: ``5``
     */
    void setMaxConcurrentDownloads(int n)
    { hash_.insert(QS("max-concurrent-downloads"), QSS(n)); }

    /*!
     * Check file integrity by validating piece hashes or a hash of entire
     * file.  This option has effect only in BitTorrent, Metalink downloads
     * with checksums or HTTP(S)/FTP downloads with
     * :option:`--checksum` option.  If
     * piece hashes are provided, this option can detect damaged portions
     * of a file and re-download them.  If a hash of entire file is
     * provided, hash check is only done when file has been already
     * download. This is determined by file length. If hash check fails,
     * file is re-downloaded from scratch.  If both piece hashes and a hash
     * of entire file are provided, only piece hashes are used. Default:
     * ``false``
     */
    void setCheckIntegrity(bool value = true)
    { hash_.insert(QS("check-integrity"), (value ? QL("true") : QL("false"))); }

    /*!
     * Continue downloading a partially downloaded file.
     * Use this option to resume a download started by a web browser or another
     * program which downloads files sequentially from the beginning.
     * Currently this option is only applicable to HTTP(S)/FTP downloads.
     */
    void setContinue(bool value = true)
    { hash_.insert(QS("continue"), (value ? QL("true") : QL("false"))); }

    //////////////////////////////////////////////////////////////////////////
    //
    // HTTP/FTP/SFTP Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Use a proxy server for all protocols.  To override a previously
     * defined proxy, use "".  You also can override this setting and specify a
     * proxy server for a particular protocol using :option:`--http-proxy`,
     * :option:`--https-proxy` and :option:`--ftp-proxy` options.  This affects all
     * downloads.
     * The format of PROXY is ``[http://][USER:PASSWORD@]HOST[:PORT]``.
     * See also `ENVIRONMENT`_ section.
     */
    void setAllProxy(const QString &proxy)
    { hash_.insert(QS("all-proxy"), proxy); }

    /*!
     * Set password for :option:`--all-proxy` option.
     */
    void setAllProxyPasswd(const QString &passwd)
    { hash_.insert(QS("all-proxy-passwd"), passwd); }

    /*!
     * Set user for :option:`--all-proxy` option.
     */
    void setAllProxyUser(const QString &user)
    { hash_.insert(QS("all-proxy-user"), user); }

    /*!
     * Set checksum. TYPE is hash type. The supported hash type is listed
     * in ``Hash Algorithms`` in ``aria2c -v``. DIGEST is hex digest.  For
     * example, setting sha-1 digest looks like this:
     * ``sha-1=0192ba11326fe2298c8cb4de616f4d4140213838`` This option applies
     * only to HTTP(S)/FTP downloads.
     */
    void setChecksum(const QString &type)
    { hash_.insert(QS("checksum"), type); }

    /*!
     * Set the connect timeout in seconds to establish connection to
     * HTTP/FTP/proxy server. After the connection is established, this
     * option makes no effect and :option:`--timeout <-t>` option is used instead.
     * Default: ``60``
     */
    void setConnectTimeout(int sec)
    { hash_.insert(QS("connect-timeout"), QSS(sec)); }

    /*!
     * If ``true`` is given, aria2 just checks whether the remote file is
     * available and doesn't download data. This option has effect on
     * HTTP/FTP download.  BitTorrent downloads are canceled if ``true`` is
     * specified.  Default: ``false``
     */
    void setDryRun(bool value = true)
    { hash_.insert(QS("dry-run"), (value ? QL("true") : QL("false"))); }

    /*!
     * Close connection if download speed is lower than or equal to this
     * value(bytes per sec).
     * ``0`` means aria2 does not have a lowest speed limit.
     * You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).
     * This option does not affect BitTorrent downloads.
     * Default: ``0``
     */
    void setLowestSpeedLimit(const QString &speed)
    { hash_.insert(QS("lowest-speed-limit"), speed); }

    /*!
     * The maximum number of connections to one server for each download.
     * Default: ``1``
     */
    void setMaxConnectionPerServer(int num)
    { hash_.insert(QS("max-connection-per-server"), QSS(num)); }

    /*!
     * If aria2 receives "file not found" status from the remote HTTP/FTP
     * servers NUM times without getting a single byte, then force the
     * download to fail. Specify ``0`` to disable this option. This options
     * is effective only when using HTTP/FTP servers.  The number of retry
     * attempt is counted toward :option:`--max-tries`, so it should be
     * configured too.
     */
    void setMaxFileNotFound(int num)
    { hash_.insert(QS("max-file-not-found"), QSS(num)); }

    /*!
     * Set number of tries. ``0`` means unlimited.
     * See also :option:`--retry-wait`.
     * Default: ``5``
     */
    void setMaxTries(int n)
    { hash_.insert(QS("max-tries"), QSS(n)); }

    /*!
     * aria2 does not split less than 2*SIZE byte range.  For example,
     * let's consider downloading 20MiB file. If SIZE is 10M, aria2 can
     * split file into 2 range [0-10MiB) and [10MiB-20MiB) and download it
     * using 2 sources(if :option:`--split <-s>` >= 2, of course).  If SIZE is 15M,
     * since 2*15M > 20MiB, aria2 does not split file and download it using
     * 1 source.  You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).
     * Possible Values: ``1M`` -``1024M`` Default: ``20M``
     */
    void setMinSplitSize(qint64 size)
    { hash_.insert(QS("min-split-size"), QSS(size)); }

    /*!
     * Specify the path to the netrc file.
     * Default: ``$(HOME)/.netrc``
     */
    void setNetrcPath(const QString &file)
    { hash_.insert(QS("netrc-path"), file); }

    /*!
     * Disables netrc support. netrc support is enabled by default.
     */
    void setNoNetrc(bool value = true)
    { hash_.insert(QS("no-netrc"), (value ? QL("true") : QL("false"))); }

    /*!
     * Specify a comma separated list of host names, domains and network addresses
     * with or without a subnet mask where no proxy should be used.
     */
    void setNoProxy(const QString &domains)
    { hash_.insert(QS("no-proxy"), domains); }

    /*!
     * The file name of the downloaded file.  It is always relative to the
     * directory given in :option:`--dir <-d>` option.  When the
     * :option:`--force-sequential <-Z>` option is used, this option is
     * ignored.
     */
    void setOut(const QString &file)
    { hash_.insert(QS("out"), file); }

    /*!
     * Set the method to use in proxy request.  METHOD is either ``get`` or
     * ``tunnel``. HTTPS downloads always use ``tunnel`` regardless of this
     * option.
     * Default: ``get``
     */
    void setProxyMethod(const QString &method)
    { hash_.insert(QS("proxy-method"), method); }

    /*!
     * Retrieve timestamp of the remote file from the remote HTTP/FTP
     * server and if it is available, apply it to the local file.
     * Default: ``false``
     */
    void setRemoteTime(bool value = true)
    { hash_.insert(QS("remote-time"), (value ? QL("true") : QL("false"))); }

    /*!
     * Reuse already used URIs if no unused URIs are left.
     * Default: ``true``
     */
    void setReuseUri(bool value = true)
    { hash_.insert(QS("reuse-uri"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set the seconds to wait between retries. When ``SEC > 0``, aria2 will
     * retry downloads when the HTTP server returns a 503 response. Default:
     * ``0``
     */
    void setRetryWait(int sec)
    { hash_.insert(QS("retry-wait"), QSS(sec)); }

    /*!
     * Specify the file name to which performance profile of the servers is
     * saved. You can load saved data using :option:`--server-stat-if` option. See
     * `Server Performance Profile`_
     * subsection below for file format.
     */
    void setServerStatOf(const QString &file)
    { hash_.insert(QS("server-stat-of"), file); }

    /*!
     * Specify the file name to load performance profile of the servers. The
     * loaded data will be used in some URI selector such as ``feedback``.
     * See also :option:`--uri-selector` option. See
     * `Server Performance Profile`_
     * subsection below for file format.
     */
    void setServerStatIf(const QString &file)
    { hash_.insert(QS("server-stat-if"), file); }

    /*!
     * Specifies timeout in seconds to invalidate performance profile of
     * the servers since the last contact to them.
     * Default: ``86400`` (24hours)
     */
    void setServerStatTimeout(int sec)
    { hash_.insert(QS("server-stat-timeout"), QSS(sec)); }

    /*!
     * Download a file using N connections.  If more than N URIs are given,
     * first N URIs are used and remaining URIs are used for backup.  If
     * less than N URIs are given, those URIs are used more than once so
     * that N connections total are made simultaneously.  The number of
     * connections to the same host is restricted by the
     * :option:`--max-connection-per-server <-x>` option.
     * See also the :option:`--min-split-size <-k>` option.
     * Default: ``5``
     */
    void setSplit(int n)
    { hash_.insert(QS("split"), QSS(n)); }

    /*!
     * Specify piece selection algorithm used in HTTP/FTP download. Piece
     * means fixed length segment which is downloaded in parallel in
     * segmented download. If ``default`` is given, aria2 selects piece so
     * that it reduces the number of establishing connection. This is
     * reasonable default behavior because establishing connection is an
     * expensive operation.  If ``inorder`` is given, aria2 selects piece
     * which has minimum index. Index=0 means first of the file. This will
     * be useful to view movie while downloading it.
     * :option:`--enable-http-pipelining` option may
     * be useful to reduce re-connection overhead.  Please note that aria2
     * honors
     * :option:`--min-split-size <-k>` option,
     * so it will be necessary to specify a reasonable value to
     * :option:`--min-split-size <-k>` option.
     * If ``random`` is given, aria2 selects piece randomly. Like
     * ``inorder``, :option:`--min-split-size <-k>` option is honored.
     * If ``geom`` is given, at the beginning aria2 selects piece which has
     * minimum index like ``inorder``, but it exponentially increasingly
     * keeps space from previously selected piece. This will reduce the
     * number of establishing connection and at the same time it will
     * download the beginning part of the file first. This will be useful
     * to view movie while downloading it.
     * Default: ``default``
     */
    void setStreamPieceSelector(const QString &selector)
    { hash_.insert(QS("stream-piece-selector"), selector); }

    /*!
     * Set timeout in seconds.
     * Default: ``60``
     */
    void setTimeout(int sec)
    { hash_.insert(QS("timeout"), QSS(sec)); }

    /*!
     * Specify URI selection algorithm. The possible values are ``inorder``,
     * ``feedback`` and ``adaptive``.  If ``inorder`` is given, URI is tried in
     * the order appeared in the URI list.  If ``feedback`` is given, aria2
     * uses download speed observed in the previous downloads and choose
     * fastest server in the URI list. This also effectively skips dead
     * mirrors. The observed download speed is a part of performance
     * profile of servers mentioned in :option:`--server-stat-of` and
     * :option:`--server-stat-if` options.  If ``adaptive`` is given, selects one of
     * the best mirrors for the first and reserved connections.  For
     * supplementary ones, it returns mirrors which has not been tested
     * yet, and if each of them has already been tested, returns mirrors
     * which has to be tested again. Otherwise, it doesn't select anymore
     * mirrors. Like ``feedback``, it uses a performance profile of servers.
     * Default: ``feedback``
     */
    void setUriSelector(const QString &selector)
    { hash_.insert(QS("uri-selector"), selector); }

    //////////////////////////////////////////////////////////////////////////
    //
    // HTTP Specific Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Use the certificate authorities in FILE to verify the peers.
     * The certificate file must be in PEM format and can contain multiple CA
     * certificates.
     * Use :option:`--check-certificate` option to enable verification.
     */
    void setCaCertificate(const QString &file)
    { hash_.insert(QS("ca-certificate"), file); }

    /*!
     * Use the client certificate in FILE. The certificate must be
     * either in PKCS12 (.p12, .pfx) or in PEM format.
     */
    void setCertificate(const QString &file)
    { hash_.insert(QS("certificate"), file); }

    /*!
     * Verify the peer using certificates specified in :option:`--ca-certificate` option.
     * Default: ``true``
     */
    void setCheckCertificate(bool value = true)
    { hash_.insert(QS("check-certificate"), (value ? QL("true") : QL("false"))); }

    /*!
     * Send ``Accept: deflate, gzip`` request header and inflate response if
     * remote server responds with ``Content-Encoding: gzip`` or
     * ``Content-Encoding: deflate``.  Default: ``false``
     */
    void setHttpAcceptGzip(bool value = true)
    { hash_.insert(QS("http-accept-gzip"), (value ? QL("true") : QL("false"))); }

    /*!
     * Send HTTP authorization header only when it is requested by the
     * server. If ``false`` is set, then authorization header is always sent
     * to the server.  There is an exception: if user name and password are
     * embedded in URI, authorization header is always sent to the server
     * regardless of this option.  Default: ``false``
     */
    void setHttpAuthChallenge(bool value = true)
    { hash_.insert(QS("http-auth-challenge"), (value ? QL("true") : QL("false"))); }

    /*!
     * Send ``Cache-Control: no-cache`` and ``Pragma: no-cache`` header to avoid
     * cached content.  If ``false`` is given, these headers are not sent
     * and you can add Cache-Control header with a directive you like
     * using :option:`--header` option. Default: ``false``
     */
    void setHttpNoCache(bool value = true)
    { hash_.insert(QS("http-no-cache"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set HTTP user. This affects all URIs.
     */
    void setHttpUser(const QString &user)
    { hash_.insert(QS("http-user"), user); }

    /*!
     * Set HTTP password. This affects all URIs.
     */
    void setHttpPasswd(const QString &passwd)
    { hash_.insert(QS("http-passwd"), passwd); }

    /*!
     * Use a proxy server for HTTP.  To override a previously defined proxy,
     * use "".
     * See also the :option:`--all-proxy` option.  This affects all http downloads.
     * The format of PROXY is ``[http://][USER:PASSWORD@]HOST[:PORT]``
     */
    void setHttpProxy(const QString &proxy)
    { hash_.insert(QS("http-proxy"), proxy); }

    /*!
     * Set password for :option:`--http-proxy`.
     */
    void setHttpProxyPasswd(const QString &passwd)
    { hash_.insert(QS("http-proxy-passwd"), passwd); }

    /*!
     * Set user for :option:`--http-proxy`.
     */
    void setHttpProxyUser(const QString &user)
    { hash_.insert(QS("http-proxy-user"), user); }

    /*!
     * Use a proxy server for HTTPS. To override a previously defined proxy,
     * use "".
     * See also the :option:`--all-proxy` option.  This affects all https download.
     * The format of PROXY is ``[http://][USER:PASSWORD@]HOST[:PORT]``
     */
    void setHttpsProxy(const QString &proxy)
    { hash_.insert(QS("https-proxy"), proxy); }

    /*!
     * Set password for :option:`--https-proxy`.
     */
    void setHttpsProxyPasswd(const QString &passwd)
    { hash_.insert(QS("https-proxy-passwd"), passwd); }

    /*!
     * Set user for :option:`--https-proxy`.
     */
    void setHttpsProxyUser(const QString &user)
    { hash_.insert(QS("https-proxy-user"), user); }

    /*!
     * Use the private key in FILE.
     * The private key must be decrypted and in PEM format.
     * The behavior when encrypted one is given is undefined.
     * See also :option:`--certificate` option.
     */
    void setPrivateKey(const QString &file)
    { hash_.insert(QS("private-key"), file); }

    /*!
     * Set an http referrer (Referer). This affects all http/https downloads.
     * If ``*`` is given, the download URI is also used as the referrer.
     * This may be useful when used together with the
     * :option:`--parameterized-uri <-P>` option.
     */
    void setReferer(const QString &referer)
    { hash_.insert(QS("referer"), referer); }

    /*!
     * Enable HTTP/1.1 persistent connection.
     * Default: ``true``
     */
    void setEnableHttpKeepAlive(bool value = true)
    { hash_.insert(QS("enable-http-keep-alive"), (value ? QL("true") : QL("false"))); }

    /*!
     * Enable HTTP/1.1 pipelining.
     * Default: ``false``
     */
    void setEnableHttpPipelining(bool value = true)
    { hash_.insert(QS("enable-http-pipelining"), (value ? QL("true") : QL("false"))); }

    /*!
     * Append HEADER to HTTP request header.
     * You can use this option repeatedly to specify more than one header:
     */
    void setHeader(const QString &header)
    { hash_.insert(QS("header"), header); }

    /*!
     * Load Cookies from FILE using the Firefox3 format (SQLite3),
     * Chromium/Google Chrome (SQLite3) and the
     * Mozilla/Firefox(1.x/2.x)/Netscape format.
     */
    void setLoadCookies(const QString &file)
    { hash_.insert(QS("load-cookies"), file); }

    /*!
     * Save Cookies to FILE in Mozilla/Firefox(1.x/2.x)/ Netscape
     * format. If FILE already exists, it is overwritten. Session Cookies
     * are also saved and their expiry values are treated as 0.  Possible
     * Values: ``/path/to/file``
     */
    void setSaveCookies(const QString &file)
    { hash_.insert(QS("save-cookies"), file); }

    /*!
     * Use HEAD method for the first request to the HTTP server.
     * Default: ``false``
     */
    void setUseHead(bool value = true)
    { hash_.insert(QS("use-head"), (value ? QL("true") : QL("false"))); }

    //////////////////////////////////////////////////////////////////////////
    //
    // FTP/SFTP Specific Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Set FTP user. This affects all URIs.
     * Default: ``anonymous``
     */
    void setFtpUser(const QString &user)
    { hash_.insert(QS("ftp-user"), user); }

    /*!
     * Set FTP password. This affects all URIs.
     * If user name is embedded but password is missing in URI, aria2 tries
     * to resolve password using .netrc. If password is found in .netrc,
     * then use it as password. If not, use the password specified in this
     * option.
     * Default: ``ARIA2USER@``
     */
    void setFtpPasswd(const QString &passwd)
    { hash_.insert(QS("ftp-passwd"), passwd); }

    /*!
     * Use the passive mode in FTP.
     * If ``false`` is given, the active mode will be used.
     * Default: ``true``
     */
    void setFtpPasv(bool value = true)
    { hash_.insert(QS("ftp-pasv"), (value ? QL("true") : QL("false"))); }

    /*!
     * Use a proxy server for FTP.  To override a previously defined proxy,
     * use "".
     * See also the :option:`--all-proxy` option.  This affects all ftp downloads.
     * The format of PROXY is ``[http://][USER:PASSWORD@]HOST[:PORT]``
     */
    void setFtpProxy(const QString &proxy)
    { hash_.insert(QS("ftp-proxy"), proxy); }

    /*!
     * Set password for :option:`--ftp-proxy` option.
     */
    void setFtpProxyPasswd(const QString &passwd)
    { hash_.insert(QS("ftp-proxy-passwd"), passwd); }

    /*!
     * Set user for :option:`--ftp-proxy` option.
     */
    void setFtpProxyUser(const QString &user)
    { hash_.insert(QS("ftp-proxy-user"), user); }

    /*!
     * Set FTP transfer type. TYPE is either ``binary`` or ``ascii``.
     * Default: ``binary``
     */
    void setFtpType(const QString &type)
    { hash_.insert(QS("ftp-type"), type); }

    /*!
     * Reuse connection in FTP.
     * Default: ``true``
     */
    void setFtpReuseConnection(bool value = true)
    { hash_.insert(QS("ftp-reuse-connection"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set checksum for SSH host public key. TYPE is hash type. The
     * supported hash type is ``sha-1`` or ``md5``. DIGEST is hex
     * digest. For example:
     * ``sha-1=b030503d4de4539dc7885e6f0f5e256704edf4c3``.  This option can
     * be used to validate server's public key when SFTP is used. If this
     * option is not set, which is default, no validation takes place.
     */
    void setSshHostKeyMd(const QString &type)
    { hash_.insert(QS("ssh-host-key-md"), type); }

    //////////////////////////////////////////////////////////////////////////
    //
    // BitTorrent/Metalink Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Set file to download by specifying its index.
     * You can find the file index using the :option:`--show-files <-S>` option.
     * Multiple indexes can be specified by using ``,``, for example: ``3,6``.
     * You can also use ``-`` to specify a range: ``1-5``.
     * ``,`` and ``-`` can be used together: ``1-5,8,9``.
     * When used with the -M option, index may vary depending on the query
     * (see *--metalink-\** options).
     */
    void setSelectFile(const QString &index)
    { hash_.insert(QS("select-file"), index); }

    /*!
     * Print file listing of ".torrent", ".meta4" and ".metalink" file and exit.
     * In case of ".torrent" file, additional information
     * (infohash, piece length, etc) is also printed.
     */
    void setShowFiles(bool value = true)
    { hash_.insert(QS("show-files"), (value ? QL("true") : QL("false"))); }

    //////////////////////////////////////////////////////////////////////////
    //
    // BitTorrent Specific Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Exclude seed only downloads when counting concurrent active
     * downloads (See :option:`-j` option).  This means that if ``-j3`` is
     * given and this option is turned on and 3 downloads are active and
     * one of those enters seed mode, then it is excluded from active
     * download count (thus it becomes 2), and the next download waiting in
     * queue gets started. But be aware that seeding item is still
     * recognized as active download in RPC method.  Default: ``false``
     */
    void setBtDetachSeedOnly(bool value = true)
    { hash_.insert(QS("bt-detach-seed-only"), (value ? QL("true") : QL("false"))); }

    /*!
     * Allow hook command invocation after hash check (see :option:`-V`
     * option) in BitTorrent download. By default, when hash check
     * succeeds, the command given by :option:`--on-bt-download-complete`
     * is executed. To disable this action, give ``false`` to this option.
     * Default: ``true``
     */
    void setBtEnableHookAfterHashCheck(bool value = true)
    { hash_.insert(QS("bt-enable-hook-after-hash-check"), (value ? QL("true") : QL("false"))); }

    /*!
     * Enable Local Peer Discovery.  If a private flag is set in a torrent,
     * aria2 doesn't use this feature for that download even if ``true`` is
     * given.  Default: ``false``
     */
    void setBtEnableLpd(bool value = true)
    { hash_.insert(QS("bt-enable-lpd"), (value ? QL("true") : QL("false"))); }

    /*!
     * Comma separated list of BitTorrent tracker's announce URI to
     * remove. You can use special value ``*`` which matches all URIs, thus
     * removes all announce URIs. When specifying ``*`` in shell
     * command-line, don't forget to escape or quote it.  See also
     * :option:`--bt-tracker` option.
     */
    void setBtExcludeTracker(const QString &uri)
    { hash_.insert(QS("bt-exclude-tracker"), uri); }

    /*!
     * Specify the external IP address to use in BitTorrent download and DHT.
     * It may be sent to BitTorrent tracker. For DHT, this option should be
     * set to report that local node is downloading a particular torrent.
     * This is critical to use DHT in a private network. Although this
     * function is named ``external``, it can accept any kind of IP
     * addresses.
     */
    void setBtExternalIp(const QString &ipaddress)
    { hash_.insert(QS("bt-external-ip"), ipaddress); }

    /*!
     * Requires BitTorrent message payload encryption with arc4.  This is a
     * shorthand of :option:`--bt-require-crypto`
     * :option:`--bt-min-crypto-level`\=arc4.  This option does not change
     * the option value of those options.  If ``true`` is given, deny
     * legacy BitTorrent handshake and only use Obfuscation handshake and
     * always encrypt message payload.  Default: ``false``
     */
    void setBtForceEncryption(bool value = true)
    { hash_.insert(QS("bt-force-encryption"), (value ? QL("true") : QL("false"))); }

    /*!
     * If ``true`` is given, after hash check using :option:`--check-integrity <-V>` option and
     * file is complete, continue to seed file. If you want to check file
     * and download it only when it is damaged or incomplete, set this
     * option to ``false``.  This option has effect only on BitTorrent download.
     * Default: ``true``
     */
    void setBtHashCheckSeed(bool value = true)
    { hash_.insert(QS("bt-hash-check-seed"), (value ? QL("true") : QL("false"))); }

    /*!
     * Before getting torrent metadata from DHT when downloading with
     * magnet link, first try to read file saved by
     * :option:`--bt-save-metadata` option.  If it is successful, then skip
     * downloading metadata from DHT.
     * Default: ``false``
     */
    void setBtLoadSavedMetadata(bool value = true)
    { hash_.insert(QS("bt-load-saved-metadata"), (value ? QL("true") : QL("false"))); }

    /*!
     * Use given interface for Local Peer Discovery. If this option is not
     * specified, the default interface is chosen. You can specify
     * interface name and IP address.  Possible Values: interface, IP
     * address
     */
    void setBtLpdInterface(const QString &itfc)
    { hash_.insert(QS("bt-lpd-interface"), itfc); }

    /*!
     * Specify maximum number of files to open in multi-file
     * BitTorrent/Metalink download globally.
     * Default: ``100``
     */
    void setBtMaxOpenFiles(int num)
    { hash_.insert(QS("bt-max-open-files"), QSS(num)); }

    /*!
     * Specify the maximum number of peers per torrent.  ``0`` means
     * unlimited.  See also :option:`--bt-request-peer-speed-limit` option.
     * Default: ``55``
     */
    void setBtMaxPeers(int num)
    { hash_.insert(QS("bt-max-peers"), QSS(num)); }

    /*!
     * Download meta data only. The file(s) described in meta data will not
     * be downloaded. This option has effect only when BitTorrent Magnet
     * URI is used. See also :option:`--bt-save-metadata` option.  Default: ``false``
     */
    void setBtMetadataOnly(bool value = true)
    { hash_.insert(QS("bt-metadata-only"), (value ? QL("true") : QL("false"))); }

    /*!
     * Removes the unselected files when download is completed in
     * BitTorrent. To select files, use
     * :option:`--select-file` option. If it is
     * not used, all files are assumed to be selected. Please use this
     * option with care because it will actually remove files from your
     * disk.
     * Default: ``false``
     */
    void setBtRemoveUnselectedFile(bool value = true)
    { hash_.insert(QS("bt-remove-unselected-file"), (value ? QL("true") : QL("false"))); }

    /*!
     * If ``true`` is given, aria2 doesn't accept and establish connection with legacy
     * BitTorrent handshake(\\19BitTorrent protocol).
     * Thus aria2 always uses Obfuscation handshake.
     * Default: ``false``
     */
    void setBtRequireCrypto(bool value = true)
    { hash_.insert(QS("bt-require-crypto"), (value ? QL("true") : QL("false"))); }

    /*!
     * If the whole download speed of every torrent is lower than SPEED,
     * aria2 temporarily increases the number of peers to try for more
     * download speed. Configuring this option with your preferred download
     * speed can increase your download speed in some cases.
     * You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).
     * Default: ``50K``
     */
    void setBtRequestPeerSpeedLimit(const QString &speed)
    { hash_.insert(QS("bt-request-peer-speed-limit"), speed); }

    /*!
     * Save meta data as ".torrent" file. This option has effect only when
     * BitTorrent Magnet URI is used.  The file name is hex encoded info
     * hash with suffix ".torrent". The directory to be saved is the same
     * directory where download file is saved. If the same file already
     * exists, meta data is not saved. See also :option:`--bt-metadata-only`
     * option. Default: ``false``
     */
    void setBtSaveMetadata(bool value = true)
    { hash_.insert(QS("bt-save-metadata"), (value ? QL("true") : QL("false"))); }

    /*!
     * Seed previously downloaded files without verifying piece hashes.
     * Default: ``false``
     */
    void setBtSeedUnverified(bool value = true)
    { hash_.insert(QS("bt-seed-unverified"), (value ? QL("true") : QL("false"))); }

    /*!
     * Stop BitTorrent download if download speed is 0 in consecutive SEC
     * seconds. If ``0`` is given, this feature is disabled.  Default: ``0``
     */
    void setBtStopTimeout(int sec)
    { hash_.insert(QS("bt-stop-timeout"), QSS(sec)); }

    /*!
     * Comma separated list of additional BitTorrent tracker's announce
     * URI. These URIs are not affected by :option:`--bt-exclude-tracker` option
     * because they are added after URIs in :option:`--bt-exclude-tracker` option are
     * removed.
     */
    void setBtTracker(const QString &uri)
    { hash_.insert(QS("bt-tracker"), uri); }

    /*!
     * Set the connect timeout in seconds to establish connection to
     * tracker. After the connection is established, this option makes no
     * effect and :option:`--bt-tracker-timeout` option is used instead.  Default:
     * ``60``
     */
    void setBtTrackerConnectTimeout(int sec)
    { hash_.insert(QS("bt-tracker-connect-timeout"), QSS(sec)); }

    /*!
     * Set the interval in seconds between tracker requests. This
     * completely overrides interval value and aria2 just uses this value
     * and ignores the min interval and interval value in the response of
     * tracker. If ``0`` is set, aria2 determines interval based on the
     * response of tracker and the download progress.  Default: ``0``
     */
    void setBtTrackerInterval(int sec)
    { hash_.insert(QS("bt-tracker-interval"), QSS(sec)); }

    /*!
     * Set timeout in seconds. Default: ``60``
     */
    void setBtTrackerTimeout(int sec)
    { hash_.insert(QS("bt-tracker-timeout"), QSS(sec)); }

    /*!
     * Set host and port as an entry point to IPv4 DHT network.
     */
    void setDhtEntryPoint(const QString &host)
    { hash_.insert(QS("dht-entry-point"), host); }

    /*!
     * Change the IPv4 DHT routing table file to PATH.
     * Default: ``$HOME/.aria2/dht.dat`` if present, otherwise
     * ``$XDG_CACHE_HOME/aria2/dht.dat``.
     */
    void setDhtFilePath(const QString &path)
    { hash_.insert(QS("dht-file-path"), path); }

    /*!
     * Set UDP listening port used by DHT(IPv4, IPv6) and UDP tracker.
     * Multiple ports can be specified by using ``,``, for example:
     * ``6881,6885``.  You can also use ``-`` to specify a range:
     * ``6881-6999``. ``,`` and ``-`` can be used together.
     * Default: ``6881-6999``
     */
    void setDhtListenPort(const QString &port)
    { hash_.insert(QS("dht-listen-port"), port); }

    /*!
     * Set timeout in seconds. Default: ``10``
     */
    void setDhtMessageTimeout(int sec)
    { hash_.insert(QS("dht-message-timeout"), QSS(sec)); }

    /*!
     * Enable IPv4 DHT functionality. It also enables UDP tracker
     * support. If a private flag is set in a torrent, aria2 doesn't use
     * DHT for that download even if ``true`` is given.  Default: ``true``
     */
    void setEnableDht(bool value = true)
    { hash_.insert(QS("enable-dht"), (value ? QL("true") : QL("false"))); }

    /*!
     * Enable Peer Exchange extension. If a private flag is set in a torrent, this
     * feature is disabled for that download even if ``true`` is given.
     * Default: ``true``
     */
    void setEnablePeerExchange(bool value = true)
    { hash_.insert(QS("enable-peer-exchange"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set file path for file with index=INDEX. You can find the file index
     * using the :option:`--show-files <-S>` option.  PATH is a relative path to the
     * path specified in :option:`--dir <-d>` option. You can use this option multiple
     * times. Using this option, you can specify the output file names of
     * BitTorrent downloads.
     */
    void setIndexOut(const QString &index)
    { hash_.insert(QS("index-out"), index); }

    /*!
     * Set TCP port number for BitTorrent downloads.
     * Multiple ports can be specified by using ``,``,  for example: ``6881,6885``.
     * You can also use ``-`` to specify a range: ``6881-6999``.
     * ``,`` and ``-`` can be used together: ``6881-6889,6999``.
     * Default: ``6881-6999``
     */
    void setListenPort(const QString &port)
    { hash_.insert(QS("listen-port"), port); }

    /*!
     * Set max overall upload speed in bytes/sec.
     * ``0`` means unrestricted.
     * You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).
     * To limit the upload speed per torrent, use :option:`--max-upload-limit <-u>` option.
     * Default: ``0``
     */
    void setMaxOverallUploadLimit(const QString &speed)
    { hash_.insert(QS("max-overall-upload-limit"), speed); }

    /*!
     * Set max upload speed per each torrent in bytes/sec.
     * ``0`` means unrestricted.
     * You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).
     * To limit the overall upload speed, use :option:`--max-overall-upload-limit` option.
     * Default: ``0``
     */
    void setMaxUploadLimit(const QString &speed)
    { hash_.insert(QS("max-upload-limit"), speed); }

    /*!
     * Specify share ratio. Seed completed torrents until share ratio reaches
     * RATIO.
     * You are strongly encouraged to specify equals or more than ``1.0`` here.
     * Specify ``0.0`` if you intend to do seeding regardless of share ratio.
     * If :option:`--seed-time` option is specified along with this option, seeding ends when
     * at least one of the conditions is satisfied.
     * Default: ``1.0``
     */
    void setSeedRatio(const QString &ratio)
    { hash_.insert(QS("seed-ratio"), ratio); }

    /*!
     * Specify seeding time in (fractional) minutes. Also see the
     * :option:`--seed-ratio` option.
     */
    void setSeedTime(const QString &minutes)
    { hash_.insert(QS("seed-time"), minutes); }

    //////////////////////////////////////////////////////////////////////////
    //
    // Metalink Specific Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Specify base URI to resolve relative URI in metalink:url and
     * metalink:metaurl element in a metalink file stored in local disk. If
     * URI points to a directory, URI must end with ``/``.
     */
    void setMetalinkBaseUri(const QString &uri)
    { hash_.insert(QS("metalink-base-uri"), uri); }

    /*!
     * The language of the file to download.
     */
    void setMetalinkLanguage(const QString &language)
    { hash_.insert(QS("metalink-language"), language); }

    /*!
     * The location of the preferred server.
     * A comma-delimited list of locations is acceptable, for example, ``jp,us``.
     */
    void setMetalinkLocation(const QString &location)
    { hash_.insert(QS("metalink-location"), location); }

    /*!
     * The operating system of the file to download.
     */
    void setMetalinkOs(const QString &os)
    { hash_.insert(QS("metalink-os"), os); }

    /*!
     * The version of the file to download.
     */
    void setMetalinkVersion(const QString &version)
    { hash_.insert(QS("metalink-version"), version); }

    /*!
     * Specify preferred protocol.
     * The possible values are ``http``, ``https``, ``ftp`` and ``none``.
     * Specify ``none`` to disable this feature.
     * Default: ``none``
     */
    void setMetalinkPreferredProtocol(const QString &proto)
    { hash_.insert(QS("metalink-preferred-protocol"), proto); }

    /*!
     * If ``true`` is given and several protocols are available for a mirror in a
     * metalink file, aria2 uses one of them.
     * Use :option:`--metalink-preferred-protocol` option to specify the preference of
     * protocol.
     * Default: ``true``
     */
    void setMetalinkEnableUniqueProtocol(bool value = true)
    { hash_.insert(QS("metalink-enable-unique-protocol"), (value ? QL("true") : QL("false"))); }

    //////////////////////////////////////////////////////////////////////////
    //
    // RPC Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Enable JSON-RPC/XML-RPC server.  It is strongly recommended to set
     * secret authorization token using :option:`--rpc-secret` option.  See
     * also :option:`--rpc-listen-port` option.  Default: ``false``
     */
    void setEnableRpc(bool value = true)
    { hash_.insert(QS("enable-rpc"), (value ? QL("true") : QL("false"))); }

    /*!
     * Pause download after added. This option is effective only when
     * :option:`--enable-rpc=true <--enable-rpc>` is given.
     * Default: ``false``
     */
    void setPause(bool value = true)
    { hash_.insert(QS("pause"), (value ? QL("true") : QL("false"))); }

    /*!
     * Pause downloads created as a result of metadata download. There are
     * 3 types of metadata downloads in aria2: (1) downloading .torrent
     * file. (2) downloading torrent metadata using magnet link. (3)
     * downloading metalink file.  These metadata downloads will generate
     * downloads using their metadata. This option pauses these subsequent
     * downloads. This option is effective only when
     * :option:`--enable-rpc=true <--enable-rpc>` is given.
     * Default: ``false``
     */
    void setPauseMetadata(bool value = true)
    { hash_.insert(QS("pause-metadata"), (value ? QL("true") : QL("false"))); }

    /*!
     * Add Access-Control-Allow-Origin header field with value ``*`` to the
     * RPC response.
     * Default: ``false``
     */
    void setRpcAllowOriginAll(bool value = true)
    { hash_.insert(QS("rpc-allow-origin-all"), (value ? QL("true") : QL("false"))); }

    /*!
     * Use the certificate in FILE for RPC server. The certificate must be
     * either in PKCS12 (.p12, .pfx) or in PEM format.
     */
    void setRpcCertificate(const QString &file)
    { hash_.insert(QS("rpc-certificate"), file); }

    /*!
     * Listen incoming JSON-RPC/XML-RPC requests on all network interfaces. If false
     * is given, listen only on local loopback interface.  Default: ``false``
     */
    void setRpcListenAll(bool value = true)
    { hash_.insert(QS("rpc-listen-all"), (value ? QL("true") : QL("false"))); }

    /*!
     * Specify a port number for JSON-RPC/XML-RPC server to listen to.  Possible
     * Values: ``1024`` -``65535`` Default: ``6800``
     */
    void setRpcListenPort(const QString &port)
    { hash_.insert(QS("rpc-listen-port"), port); }

    /*!
     * Set max size of JSON-RPC/XML-RPC request. If aria2 detects the request is
     * more than SIZE bytes, it drops connection. Default: ``2M``
     */
    void setRpcMaxRequestSize(qint64 size)
    { hash_.insert(QS("rpc-max-request-size"), QSS(size)); }

    /*!
     * Set JSON-RPC/XML-RPC password.
     */
    void setRpcPasswd(const QString &passwd)
    { hash_.insert(QS("rpc-passwd"), passwd); }

    /*!
     * Use the private key in FILE for RPC server.  The private key must be
     * decrypted and in PEM format. Use :option:`--rpc-secure` option to
     * enable encryption. See also :option:`--rpc-certificate` option.
     */
    void setRpcPrivateKey(const QString &file)
    { hash_.insert(QS("rpc-private-key"), file); }

    /*!
     * Save the uploaded torrent or metalink meta data in the directory
     * specified by :option:`--dir` option. The file name consists of SHA-1
     * hash hex string of meta data plus extension. For torrent, the
     * extension is '.torrent'. For metalink, it is '.meta4'.  If false is
     * given to this option, the downloads added by
     * :func:`aria2.addTorrent` or :func:`aria2.addMetalink` will not be
     * saved by :option:`--save-session` option. Default: ``true``
     */
    void setRpcSaveUploadMetadata(bool value = true)
    { hash_.insert(QS("rpc-save-upload-metadata"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set RPC secret authorization token. Read :ref:`rpc_auth` to know
     * how this option value is used.
     */
    void setRpcSecret(const QString &token)
    { hash_.insert(QS("rpc-secret"), token); }

    /*!
     * RPC transport will be encrypted by SSL/TLS.  The RPC clients must
     * use https scheme to access the server. For WebSocket client, use wss
     * scheme. Use :option:`--rpc-certificate` and
     * :option:`--rpc-private-key` options to specify the server
     * certificate and private key.
     */
    void setRpcSecure(bool value = true)
    { hash_.insert(QS("rpc-secure"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set JSON-RPC/XML-RPC user.
     */
    void setRpcUser(const QString &user)
    { hash_.insert(QS("rpc-user"), user); }

    //////////////////////////////////////////////////////////////////////////
    //
    // Advanced Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Restart download from scratch if the corresponding control file
     * doesn't exist.  See also :option:`--auto-file-renaming` option.  Default:
     * ``false``
     */
    void setAllowOverwrite(bool value = true)
    { hash_.insert(QS("allow-overwrite"), (value ? QL("true") : QL("false"))); }

    /*!
     * If false is given, aria2 aborts download when a piece length is different
     * from one in a control file.
     * If true is given, you can proceed but some download progress will be lost.
     * Default: ``false``
     */
    void setAllowPieceLengthChange(bool value = true)
    { hash_.insert(QS("allow-piece-length-change"), (value ? QL("true") : QL("false"))); }

    /*!
     * Always resume download. If ``true`` is given, aria2 always tries to
     * resume download and if resume is not possible, aborts download.  If
     * ``false`` is given, when all given URIs do not support resume or aria2
     * encounters ``N`` URIs which does not support resume (``N`` is the value
     * specified using :option:`--max-resume-failure-tries` option), aria2
     * downloads file from scratch.  See :option:`--max-resume-failure-tries`
     * option. Default: ``true``
     */
    void setAlwaysResume(bool value = true)
    { hash_.insert(QS("always-resume"), (value ? QL("true") : QL("false"))); }

    /*!
     * Enable asynchronous DNS.
     * Default: ``true``
     */
    void setAsyncDns(bool value = true)
    { hash_.insert(QS("async-dns"), (value ? QL("true") : QL("false"))); }

    /*!
     * Comma separated list of DNS server address used in asynchronous DNS
     * resolver. Usually asynchronous DNS resolver reads DNS server
     * addresses from ``/etc/resolv.conf``. When this option is used, it uses
     * DNS servers specified in this option instead of ones in
     * ``/etc/resolv.conf``. You can specify both IPv4 and IPv6 address. This
     * option is useful when the system does not have ``/etc/resolv.conf`` and
     * user does not have the permission to create it.
     */
    void setAsyncDnsServer(const QString &ipaddress)
    { hash_.insert(QS("async-dns-server"), ipaddress); }

    /*!
     * Rename file name if the same file already exists.
     * This option works only in HTTP(S)/FTP download.
     * The new file name has a dot and a number(1..9999) appended after the
     * name, but before the file extension, if any.
     * Default: ``true``
     */
    void setAutoFileRenaming(bool value = true)
    { hash_.insert(QS("auto-file-renaming"), (value ? QL("true") : QL("false"))); }

    /*!
     * Save a control file(\*.aria2) every SEC seconds.
     * If ``0`` is given, a control file is not saved during download. aria2 saves a
     * control file when it stops regardless of the value.
     * The possible values are between ``0`` to ``600``.
     * Default: ``60``
     */
    void setAutoSaveInterval(int sec)
    { hash_.insert(QS("auto-save-interval"), QSS(sec)); }

    /*!
     * Download file only when the local file is older than remote
     * file. This function only works with HTTP(S) downloads only.  It does
     * not work if file size is specified in Metalink. It also ignores
     * Content-Disposition header.  If a control file exists, this option
     * will be ignored.  This function uses If-Modified-Since header to get
     * only newer file conditionally. When getting modification time of
     * local file, it uses user supplied file name (see :option:`--out <-o>` option) or
     * file name part in URI if :option:`--out <-o>` is not specified.
     * To overwrite existing file, :option:`--allow-overwrite` is required.
     * Default: ``false``
     */
    void setConditionalGet(bool value = true)
    { hash_.insert(QS("conditional-get"), (value ? QL("true") : QL("false"))); }

    /*!
     * Change the configuration file path to PATH.
     * Default: ``$HOME/.aria2/aria2.conf`` if present, otherwise
     * ``$XDG_CONFIG_HOME/aria2/aria2.conf``.
     */
    void setConfPath(const QString &path)
    { hash_.insert(QS("conf-path"), path); }

    /*!
     * Set log level to output to console.  LEVEL is either ``debug``,
     * ``info``, ``notice``, ``warn`` or ``error``.  Default: ``notice``
     */
    void setConsoleLogLevel(const QString &level)
    { hash_.insert(QS("console-log-level"), level); }

    /*!
     * Run as daemon. The current working directory will be changed to ``/``
     * and standard input, standard output and standard error will be
     * redirected to ``/dev/null``. Default: ``false``
     */
    void setDaemon(bool value = true)
    { hash_.insert(QS("daemon"), (value ? QL("true") : QL("false"))); }

    /*!
     * If ``true`` is given, aria2 does not read all URIs and options from file
     * specified by :option:`--input-file <-i>` option at startup,
     * but it reads one by one when it
     * needs later. This may reduce memory usage if input file contains a
     * lot of URIs to download.  If ``false`` is given, aria2 reads all URIs
     * and options at startup.
     * Default: ``false``
     */
    void setDeferredInput(bool value = true)
    { hash_.insert(QS("deferred-input"), (value ? QL("true") : QL("false"))); }

    /*!
     * Enable disk cache. If SIZE is ``0``, the disk cache is
     * disabled. This feature caches the downloaded data in memory, which
     * grows to at most SIZE bytes. The cache storage is created for aria2
     * instance and shared by all downloads. The one advantage of the disk
     * cache is reduce the disk I/O because the data are written in larger
     * unit and it is reordered by the offset of the file.  If hash
     * checking is involved and the data are cached in memory, we don't
     * need to read them from the disk.  SIZE can include ``K`` or ``M``
     * (1K = 1024, 1M = 1024K). Default: ``16M``
     */
    void setDiskCache(qint64 size)
    { hash_.insert(QS("disk-cache"), QSS(size)); }

    /*!
     * This option changes the way ``Download Results`` is formatted. If
     * OPT is ``default``, print GID, status, average download speed and
     * path/URI. If multiple files are involved, path/URI of first
     * requested file is printed and remaining ones are omitted.  If OPT is
     * ``full``, print GID, status, average download speed, percentage of
     * progress and path/URI. The percentage of progress and path/URI are
     * printed for each requested file in each row.  If OPT is ``hide``,
     * ``Download Results`` is hidden.
     * Default: ``default``
     */
    void setDownloadResult(const QString &opt)
    { hash_.insert(QS("download-result"), opt); }

    /*!
     * Set DSCP value in outgoing IP packets of BitTorrent traffic for
     * QoS. This parameter sets only DSCP bits in TOS field of IP packets,
     * not the whole field. If you take values from
     * * /usr/include/netinet/ip.h * divide them by 4 (otherwise values would
     * be incorrect, e.g. your ``CS1`` class would turn into ``CS4``). If
     * you take commonly used values from RFC, network vendors'
     * documentation, Wikipedia or any other source, use them as they are.
     */
    void setDscp(const QString &dscp)
    { hash_.insert(QS("dscp"), dscp); }

    /*!
     * Set the soft limit of open file descriptors.
     * This open will only have effect when:
     */
    void setRlimitNofile(int num)
    { hash_.insert(QS("rlimit-nofile"), QSS(num)); }

    /*!
     * Enable color output for a terminal.
     * Default: ``true``
     */
    void setEnableColor(bool value = true)
    { hash_.insert(QS("enable-color"), (value ? QL("true") : QL("false"))); }

    /*!
     * Map files into memory. This option may not work if the file space
     * is not pre-allocated. See :option:`--file-allocation`.
     */
    void setEnableMmap(bool value = true)
    { hash_.insert(QS("enable-mmap"), (value ? QL("true") : QL("false"))); }

    /*!
     * Specify the method for polling events.  The possible values are
     * ``epoll``, ``kqueue``, ``port``, ``poll`` and ``select``.  For each ``epoll``,
     * ``kqueue``, ``port`` and ``poll``, it is available if system supports it.
     * ``epoll`` is available on recent Linux. ``kqueue`` is available on
     * various \*BSD systems including Mac OS X. ``port`` is available on Open
     * Solaris. The default value may vary depending on the system you use.
     */
    void setEventPoll(const QString &poll)
    { hash_.insert(QS("event-poll"), poll); }

    /*!
     * Specify file allocation method.
     * ``none`` doesn't pre-allocate file space. ``prealloc`` pre-allocates file space
     * before download begins. This may take some time depending on the size of the
     * file.
     * If you are using newer file systems such as ext4
     * (with extents support), btrfs, xfs or NTFS(MinGW build only), ``falloc`` is
     * your best choice. It allocates large(few GiB)
     * files almost instantly. Don't use ``falloc`` with
     * legacy file systems such as ext3 and FAT32 because it takes
     * almost same time as ``prealloc`` and it blocks aria2
     * entirely until allocation finishes. ``falloc`` may
     * not be available if your system doesn't have
     * :manpage:`posix_fallocate(3)` function.
     * ``trunc`` uses :manpage:`ftruncate(2)` system call or
     * platform-specific counterpart to truncate a file to a specified
     * length.
     */
    void setFileAllocation(const QString &method)
    { hash_.insert(QS("file-allocation"), method); }

    /*!
     * Save download with :option:`--save-session <--save-session>` option
     * even if the download is completed or removed. This option also saves
     * control file in that situations. This may be useful to save
     * BitTorrent seeding which is recognized as completed state.
     * Default: ``false``
     */
    void setForceSave(bool value = true)
    { hash_.insert(QS("force-save"), (value ? QL("true") : QL("false"))); }

    /*!
     * Save download with :option:`--save-session <--save-session>` option
     * even if the file was not found on the server. This option also saves
     * control file in that situations.
     * Default: ``true``
     */
    void setSaveNotFound(bool value = true)
    { hash_.insert(QS("save-not-found"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set GID manually. aria2 identifies each download by the ID called
     * GID. The GID must be hex string of 16 characters, thus [0-9a-fA-F]
     * are allowed and leading zeros must not be stripped. The GID all 0 is
     * reserved and must not be used. The GID must be unique, otherwise
     * error is reported and the download is not added.  This option is
     * useful when restoring the sessions saved using
     * :option:`--save-session <--save-session>` option. If this option is
     * not used, new GID is generated by aria2.
     */
    void setGid(const QString &gid)
    { hash_.insert(QS("gid"), gid); }

    /*!
     * If ``true`` is given, after hash check using
     * :option:`--check-integrity <-V>` option,
     * abort download whether or not download is complete.
     * Default: ``false``
     */
    void setHashCheckOnly(bool value = true)
    { hash_.insert(QS("hash-check-only"), (value ? QL("true") : QL("false"))); }

    /*!
     * Print sizes and speed in human readable format (e.g., 1.2Ki, 3.4Mi)
     * in the console readout. Default: ``true``
     */
    void setHumanReadable(bool value = true)
    { hash_.insert(QS("human-readable"), (value ? QL("true") : QL("false"))); }

    /*!
     * Bind sockets to given interface. You can specify interface name, IP
     * address and host name.
     * Possible Values: interface, IP address, host name
     */
    void setInterface(const QString &itfc)
    { hash_.insert(QS("interface"), itfc); }

    /*!
     * Keep unfinished download results even if doing so exceeds
     * :option:`--max-download-result`.  This is useful if all unfinished
     * downloads must be saved in session file (see
     * :option:`--save-session` option).  Please keep in mind that there is
     * no upper bound to the number of unfinished download result to keep.
     * If that is undesirable, turn this option off.  Default: ``true``
     */
    void setKeepUnfinishedDownloadResult(bool value = true)
    { hash_.insert(QS("keep-unfinished-download-result"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set maximum number of download result kept in memory. The download
     * results are completed/error/removed downloads. The download results
     * are stored in FIFO queue and it can store at most NUM download
     * results. When queue is full and new download result is created,
     * oldest download result is removed from the front of the queue and
     * new one is pushed to the back. Setting big number in this option may
     * result high memory consumption after thousands of
     * downloads. Specifying 0 means no download result is kept.  Note that
     * unfinished downloads are kept in memory regardless of this option
     * value. See :option:`--keep-unfinished-download-result` option.
     * Default: ``1000``
     */
    void setMaxDownloadResult(int num)
    { hash_.insert(QS("max-download-result"), QSS(num)); }

    /*!
     * Set the maximum file size to enable mmap (see
     * :option:`--enable-mmap` option). The file size is determined by the
     * sum of all files contained in one download. For example, if a
     * download contains 5 files, then file size is the total size of those
     * files. If file size is strictly greater than the size specified in
     * this option, mmap will be disabled.
     * Default: ``9223372036854775807``
     */
    void setMaxMmapLimit(qint64 size)
    { hash_.insert(QS("max-mmap-limit"), QSS(size)); }

    /*!
     * When used with :option:`--always-resume=false, <--always-resume>` aria2 downloads file from
     * scratch when aria2 detects N number of URIs that does not support
     * resume. If N is ``0``, aria2 downloads file from scratch when all
     * given URIs do not support resume.  See :option:`--always-resume` option.
     * Default: ``0``
     */
    void setMaxResumeFailureTries(int n)
    { hash_.insert(QS("max-resume-failure-tries"), QSS(n)); }

    /*!
     * Specify minimum SSL/TLS version to enable.
     * Possible Values: ``TLSv1.1``, ``TLSv1.2``, ``TLSv1.3``
     * Default: ``TLSv1.2``
     */
    void setMinTlsVersion(const QString &version)
    { hash_.insert(QS("min-tls-version"), version); }

    /*!
     * Comma separated list of interfaces to bind sockets to. Requests will
     * be splited among the interfaces to achieve link aggregation. You can
     * specify interface name, IP address and hostname. If
     * :option:`--interface` is used, this option will be ignored.
     * Possible Values: interface, IP address, hostname
     */
    void setMultipleInterface(const QString &interfaces)
    { hash_.insert(QS("multiple-interface"), interfaces); }

    /*!
     * Set log level to output.
     * LEVEL is either ``debug``, ``info``, ``notice``, ``warn`` or ``error``.
     * Default: ``debug``
     */
    void setLogLevel(const QString &level)
    { hash_.insert(QS("log-level"), level); }

    /*!
     * Set a piece length for HTTP/FTP downloads. This is the boundary when
     * aria2 splits a file. All splits occur at multiple of this
     * length. This option will be ignored in BitTorrent downloads.  It
     * will be also ignored if Metalink file contains piece hashes.
     * Default: ``1M``
     */
    void setPieceLength(const QString &length)
    { hash_.insert(QS("piece-length"), length); }

    /*!
     * Show console readout. Default: ``true``
     */
    void setShowConsoleReadout(bool value = true)
    { hash_.insert(QS("show-console-readout"), (value ? QL("true") : QL("false"))); }

    /*!
     * Redirect all console output that would be otherwise printed in
     * stdout to stderr.  Default: ``false``
     */
    void setStderr(bool value = true)
    { hash_.insert(QS("stderr"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set interval in seconds to output download progress summary.
     * Setting ``0`` suppresses the output.
     * Default: ``60``
     */
    void setSummaryInterval(int sec)
    { hash_.insert(QS("summary-interval"), QSS(sec)); }

    /*!
     * Fetch URIs in the command-line sequentially and download each URI in a
     * separate session, like the usual command-line download utilities.
     * Default: ``false``
     */
    void setForceSequential(bool value = true)
    { hash_.insert(QS("force-sequential"), (value ? QL("true") : QL("false"))); }

    /*!
     * Set max overall download speed in bytes/sec.  ``0`` means
     * unrestricted.  You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).  To
     * limit the download speed per download, use :option:`--max-download-limit`
     * option.  Default: ``0``
     */
    void setMaxOverallDownloadLimit(const QString &speed)
    { hash_.insert(QS("max-overall-download-limit"), speed); }

    /*!
     * Set max download speed per each download in bytes/sec. ``0`` means
     * unrestricted.  You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).  To
     * limit the overall download speed, use :option:`--max-overall-download-limit`
     * option.  Default: ``0``
     */
    void setMaxDownloadLimit(const QString &speed)
    { hash_.insert(QS("max-download-limit"), speed); }

    /*!
     * Disable loading aria2.conf file.
     */
    void setNoConf(bool value = true)
    { hash_.insert(QS("no-conf"), (value ? QL("true") : QL("false"))); }

    /*!
     * No file allocation is made for files whose size is smaller than SIZE.
     * You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).
     * Default: ``5M``
     */
    void setNoFileAllocationLimit(qint64 size)
    { hash_.insert(QS("no-file-allocation-limit"), QSS(size)); }

    /*!
     * Enable parameterized URI support.
     * You can specify set of parts: ``http://{sv1,sv2,sv3}/foo.iso``.
     * Also you can specify numeric sequences with step counter:
     * ``http://host/image[000-100:2].img``.
     * A step counter can be omitted.
     * If all URIs do not point to the same file, such as the second example above,
     * -Z option is required.
     * Default: ``false``
     */
    void setParameterizedUri(bool value = true)
    { hash_.insert(QS("parameterized-uri"), (value ? QL("true") : QL("false"))); }

    /*!
     * Make aria2 quiet (no console output).
     * Default: ``false``
     */
    void setQuiet(bool value = true)
    { hash_.insert(QS("quiet"), (value ? QL("true") : QL("false"))); }

    /*!
     * Validate chunk of data by calculating checksum while downloading a file if
     * chunk checksums are provided.
     * Default: ``true``
     */
    void setRealtimeChunkChecksum(bool value = true)
    { hash_.insert(QS("realtime-chunk-checksum"), (value ? QL("true") : QL("false"))); }

    /*!
     * Remove control file before download. Using with
     * :option:`--allow-overwrite=true, <--allow-overwrite>` download always starts from
     * scratch. This will be useful for users behind proxy server which
     * disables resume.
     */
    void setRemoveControlFile(bool value = true)
    { hash_.insert(QS("remove-control-file"), (value ? QL("true") : QL("false"))); }

    /*!
     * Save error/unfinished downloads to FILE on exit.  You can pass this
     * output file to aria2c with :option:`--input-file <-i>` option on
     * restart. If you like the output to be gzipped append a .gz extension to
     * the file name.
     * Please note that downloads added by
     * :func:`aria2.addTorrent` and :func:`aria2.addMetalink` RPC method
     * and whose meta data could not be saved as a file are not saved.
     * Downloads removed using :func:`aria2.remove` and
     * :func:`aria2.forceRemove` will not be saved. GID is also saved with
     * :option:`gid <--gid>`, but there are some restrictions, see below.
     */
    void setSaveSession(const QString &file)
    { hash_.insert(QS("save-session"), file); }

    /*!
     * Save error/unfinished downloads to a file specified by
     * :option:`--save-session` option every SEC seconds. If ``0`` is
     * given, file will be saved only when aria2 exits. Default: ``0``
     */
    void setSaveSessionInterval(int sec)
    { hash_.insert(QS("save-session-interval"), QSS(sec)); }

    /*!
     * Set the maximum socket receive buffer in bytes.  Specifying ``0``
     * will disable this option. This value will be set to socket file
     * descriptor using ``SO_RCVBUF`` socket option with ``setsockopt()``
     * call.  Default: ``0``
     */
    void setSocketRecvBufferSize(qint64 size)
    { hash_.insert(QS("socket-recv-buffer-size"), QSS(size)); }

    /*!
     * Stop application after SEC seconds has passed.
     * If ``0`` is given, this feature is disabled.
     * Default: ``0``
     */
    void setStop(int sec)
    { hash_.insert(QS("stop"), QSS(sec)); }

    /*!
     * Stop application when process PID is not running.  This is useful if
     * aria2 process is forked from a parent process. The parent process
     * can fork aria2 with its own pid and when parent process exits for
     * some reason, aria2 can detect it and shutdown itself.
     */
    void setStopWithProcess(int pid)
    { hash_.insert(QS("stop-with-process"), QSS(pid)); }

    /*!
     * Truncate console readout to fit in a single line.
     * Default: ``true``
     */
    void setTruncateConsoleReadout(bool value = true)
    { hash_.insert(QS("truncate-console-readout"), (value ? QL("true") : QL("false"))); }


    /*!
     * Optimizes the number of concurrent downloads according to the bandwidth available.
     * aria2 uses the download speed observed in the previous downloads to adapt the number
     * of downloads launched in parallel according to the rule N = A + B Log10(speed in Mbps).
     * The coefficients A and B can be customized in the option arguments with A and B separated
     * by a colon. The default values (A=5, B=25) lead to using typically 5 parallel downloads
     * on 1Mbps networks and above 50 on 100Mbps networks. The number of parallel downloads
     * remains constrained under the maximum defined by the --max-concurrent-downloads parameter.
     * Default: ``false``
     */
    void setOptimizeConcurrentDownloads(bool value = true)
    { hash_.insert(QS("optimize-concurrent-downloads"), (value ? QL("true") : QL("false"))); }

    void setOption(const QString &name, const QVariant &value)
    {
        hash_.insert(name, value.toString());
    }

    const QVariantHash &options() const { return hash_; }

private:
    QVariantHash hash_;
};
