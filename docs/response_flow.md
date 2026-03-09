flowchart TD
    classDef done fill:#22c55e,color:#fff
    IN["📥 Request chega no Handler    method · uri · *Location · *ServerConf"]

    IN --> M{"método permitido?  Location::allowed_methods"}
    M -->|não| E405["🔴 405 Method Not Allowed"]

    M -->|sim| PATH["resolver path real
        root + uri"]
    PATH --> TRAV{"path traversal?
        ../  fora do root"}
    TRAV -->|sim| E403a["🔴 403 Forbidden"]

    TRAV -->|não| EXIST{"path existe?   stat()"}
    EXIST -->|não| E404["🔴 404 Not Found"]

    EXIST -->|sim| PERM{"permissão de leitura? - access() / stat.st_mode"}
    PERM -->|não| E403b["🔴 403 Forbidden"]

    PERM -->|sim| TYPE{"é diretório?S_ISDIR()"}

    %% Diretório
    TYPE -->|diretório| IDX{"tem index file? - Location::index_files"}
    IDX -->|sim| REDIR{"URI termina     em '/'?"}
    REDIR -->|não| E301["🔵 301 Redirect     URI + '/'"]
    REDIR -->|sim| SERVE_IDX["serve index file - → fluxo de arquivo"]

    IDX -->|não| AUTO{"autoindex on?     Location::autoindex"}
    AUTO -->|sim| LISTING["gera autoindex HTML     readdir() listing"]
    AUTO -->|não| E403c["🔴 403 Forbidden"]

    %% Arquivo
    TYPE -->|arquivo| CGI{"é CGI?\ncgi_pass configurado?"}
    CGI -->|sim| CGI_MOD["⚙️ CGI Module\nfork · execve · pipe"]
    CGI -->|não| READ["lê conteúdo     ifstream binary"]
    CGI_MOD --> R200["🟢 200 OK"]
    READ --> MIME["detecta MIME type     extensão → Content-Type"]
    MIME --> R200

    SERVE_IDX --> READ
    LISTING --> R200

    %% Erros convergem
    E405 & E403a & E404 & E403b & E403c --> ERR_PAGE{"error_page     configurado?"}
    ERR_PAGE -->|sim| CUSTOM["serve arquivo     de erro customizado"]
    ERR_PAGE -->|não| DEFAULT["gera HTML     de erro padrão"]

    %% Response builder
    R200 & CUSTOM & DEFAULT & E301 --> BUILD

    subgraph BUILD["HttpResponse builder"]
        direction LR
        SL["status line     HTTP/1.1 XYZ"] --> HD["Content-Type     Content-Length     Connection"] --> BD["body bytes"]
    end
    class ERR_PAGE,CUSTOM,DEFAULT done
    BUILD --> OUT["write_buffer → Part 1     send()"]