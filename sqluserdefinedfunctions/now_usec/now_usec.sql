DROP FUNCTION IF EXISTS now_usec;

CREATE FUNCTION now_usec RETURNS string SONAME 'now_usec.so';

