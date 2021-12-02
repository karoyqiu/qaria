/*! ***********************************************************************************************
 *
 * \file        aria2optionsbuilder.h
 * \brief       OptionsBuilder 类头文件。
 * \version     0.1
 * \date        2021-12-02
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
    { hash_.insert(QS("max-concurrent-downloads"), n); }

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
    { hash_.insert(QS("connect-timeout"), sec); }

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
    { hash_.insert(QS("max-connection-per-server"), num); }

    /*!
     * If aria2 receives "file not found" status from the remote HTTP/FTP
     * servers NUM times without getting a single byte, then force the
     * download to fail. Specify ``0`` to disable this option. This options
     * is effective only when using HTTP/FTP servers.  The number of retry
     * attempt is counted toward :option:`--max-tries`, so it should be
     * configured too.
     */
    void setMaxFileNotFound(int num)
    { hash_.insert(QS("max-file-not-found"), num); }

    /*!
     * Set number of tries. ``0`` means unlimited.
     * See also :option:`--retry-wait`.
     * Default: ``5``
     */
    void setMaxTries(int n)
    { hash_.insert(QS("max-tries"), n); }

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
    { hash_.insert(QS("min-split-size"), size); }

    /*!
     * Specify the path to the netrc file.
     * Default: ``$(HOME)/.netrc``
     */
    void setNetrcPath(const QString &file)
    { hash_.insert(QS("netrc-path"), file); }

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
     * Set the seconds to wait between retries. When ``SEC > 0``, aria2 will
     * retry downloads when the HTTP server returns a 503 response. Default:
     * ``0``
     */
    void setRetryWait(int sec)
    { hash_.insert(QS("retry-wait"), sec); }

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
    { hash_.insert(QS("server-stat-timeout"), sec); }

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
    { hash_.insert(QS("split"), n); }

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
    { hash_.insert(QS("timeout"), sec); }

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

    //////////////////////////////////////////////////////////////////////////
    //
    // BitTorrent Specific Options
    //
    //////////////////////////////////////////////////////////////////////////

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
    { hash_.insert(QS("bt-max-open-files"), num); }

    /*!
     * Specify the maximum number of peers per torrent.  ``0`` means
     * unlimited.  See also :option:`--bt-request-peer-speed-limit` option.
     * Default: ``55``
     */
    void setBtMaxPeers(int num)
    { hash_.insert(QS("bt-max-peers"), num); }

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
     * Stop BitTorrent download if download speed is 0 in consecutive SEC
     * seconds. If ``0`` is given, this feature is disabled.  Default: ``0``
     */
    void setBtStopTimeout(int sec)
    { hash_.insert(QS("bt-stop-timeout"), sec); }

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
    { hash_.insert(QS("bt-tracker-connect-timeout"), sec); }

    /*!
     * Set the interval in seconds between tracker requests. This
     * completely overrides interval value and aria2 just uses this value
     * and ignores the min interval and interval value in the response of
     * tracker. If ``0`` is set, aria2 determines interval based on the
     * response of tracker and the download progress.  Default: ``0``
     */
    void setBtTrackerInterval(int sec)
    { hash_.insert(QS("bt-tracker-interval"), sec); }

    /*!
     * Set timeout in seconds. Default: ``60``
     */
    void setBtTrackerTimeout(int sec)
    { hash_.insert(QS("bt-tracker-timeout"), sec); }

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
    { hash_.insert(QS("dht-message-timeout"), sec); }

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

    //////////////////////////////////////////////////////////////////////////
    //
    // RPC Options
    //
    //////////////////////////////////////////////////////////////////////////

    /*!
     * Use the certificate in FILE for RPC server. The certificate must be
     * either in PKCS12 (.p12, .pfx) or in PEM format.
     */
    void setRpcCertificate(const QString &file)
    { hash_.insert(QS("rpc-certificate"), file); }

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
    { hash_.insert(QS("rpc-max-request-size"), size); }

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
     * Set RPC secret authorization token. Read :ref:`rpc_auth` to know
     * how this option value is used.
     */
    void setRpcSecret(const QString &token)
    { hash_.insert(QS("rpc-secret"), token); }

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
     * Save a control file(\*.aria2) every SEC seconds.
     * If ``0`` is given, a control file is not saved during download. aria2 saves a
     * control file when it stops regardless of the value.
     * The possible values are between ``0`` to ``600``.
     * Default: ``60``
     */
    void setAutoSaveInterval(int sec)
    { hash_.insert(QS("auto-save-interval"), sec); }

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
    { hash_.insert(QS("disk-cache"), size); }

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
    { hash_.insert(QS("rlimit-nofile"), num); }

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
     * Bind sockets to given interface. You can specify interface name, IP
     * address and host name.
     * Possible Values: interface, IP address, host name
     */
    void setInterface(const QString &itfc)
    { hash_.insert(QS("interface"), itfc); }

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
    { hash_.insert(QS("max-download-result"), num); }

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
    { hash_.insert(QS("max-mmap-limit"), size); }

    /*!
     * When used with :option:`--always-resume=false, <--always-resume>` aria2 downloads file from
     * scratch when aria2 detects N number of URIs that does not support
     * resume. If N is ``0``, aria2 downloads file from scratch when all
     * given URIs do not support resume.  See :option:`--always-resume` option.
     * Default: ``0``
     */
    void setMaxResumeFailureTries(int n)
    { hash_.insert(QS("max-resume-failure-tries"), n); }

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
     * Set interval in seconds to output download progress summary.
     * Setting ``0`` suppresses the output.
     * Default: ``60``
     */
    void setSummaryInterval(int sec)
    { hash_.insert(QS("summary-interval"), sec); }

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
     * No file allocation is made for files whose size is smaller than SIZE.
     * You can append ``K`` or ``M`` (1K = 1024, 1M = 1024K).
     * Default: ``5M``
     */
    void setNoFileAllocationLimit(qint64 size)
    { hash_.insert(QS("no-file-allocation-limit"), size); }

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
    { hash_.insert(QS("save-session-interval"), sec); }

    /*!
     * Set the maximum socket receive buffer in bytes.  Specifying ``0``
     * will disable this option. This value will be set to socket file
     * descriptor using ``SO_RCVBUF`` socket option with ``setsockopt()``
     * call.  Default: ``0``
     */
    void setSocketRecvBufferSize(qint64 size)
    { hash_.insert(QS("socket-recv-buffer-size"), size); }

    /*!
     * Stop application after SEC seconds has passed.
     * If ``0`` is given, this feature is disabled.
     * Default: ``0``
     */
    void setStop(int sec)
    { hash_.insert(QS("stop"), sec); }

    /*!
     * Stop application when process PID is not running.  This is useful if
     * aria2 process is forked from a parent process. The parent process
     * can fork aria2 with its own pid and when parent process exits for
     * some reason, aria2 can detect it and shutdown itself.
     */
    void setStopWithProcess(int pid)
    { hash_.insert(QS("stop-with-process"), pid); }


    const QVariantHash &options() const { return hash_; }

private:
    QVariantHash hash_;
};
