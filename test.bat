FOR %%? IN (file_to_be_queried) DO (
    ECHO File Name Only       : %%~n?
    ECHO File Extension       : %%~x?
    ECHO Name in 8.3 notation : %%~sn?
    ECHO File Attributes      : %%~a?
    ECHO Located on Drive     : %%~d?
    ECHO File Size            : %%~z?
    ECHO Last-Modified Date   : %%~t?
    ECHO Parent Folder        : %%~dp?
    ECHO Fully Qualified Path : %%~f?
    ECHO FQP in 8.3 notation  : %%~sf?
    ECHO Location in the PATH : %%~dp$PATH:?
)