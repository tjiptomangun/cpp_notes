#include <stdio.h>
#include <stdlib.h>
int main (int argc , char ** argv)
{
	char szMail[1024];
	sprintf(szMail,"echo \"To:%s\nFrom:X/Link XAL Mailer<%s>\nCc:%s\nSubject:%s\nDengan Hormat,\n\nBerikut adalah insiden yang terjadi pada %s\nAlarm : %s\n.\n\nSalam,\nSistem X/Link Alert Mailer\n\n\" | /usr/sbin/sendmail -f%s %s","henky.ac@gmail.com","henky@infolink.co.id","henky@infolink.co.id","some alarm","00:00:00","Semua mati","henky@infolink.co.id","henky.ac@gmail.com");
/*
	sprintf(szMail,"echo \"To: %s\x0a Cc: X/Link XAL Mailer <%s>\x0a  From: X/Link XAL Mailer <%s>\x0a  Subject: %s\x0aMime-Version: 1.0\x0a Content-Type: text/plain; charset=\\\\\"us-ascii\\\\\"; format=flowed\x0aContent-Transfer-Encoding: 7bit\x0a\x0aDengan Hormat,\x0a\x0aBerikut adalah insiden yang terjadi pada %s\x0aAlarm : %s\x0a.\x0a\x0aSalam,\x0aSistem X/Link Alert Mailer\x0a \x0a\" | /usr/sbin/sendmail -f%s %s","henky@infolink.co.id","henky@infolink.co.id","henky@infolink.co.id","some alarm","00:00:00","Semua mati","henky@infolink.co.id","henky@infolink.co.id");
*/
        printf(szMail);
	system(szMail);
}
