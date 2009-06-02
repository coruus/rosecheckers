#!/usr/bin/perl

BEGIN {
  unshift(@INC, "/afs/andrew.cmu.edu/usr16/jpincar/cert/WWW-Mechanize-1.30/lib/");
}

$version = "v2.5";

use WWW::Mechanize;
my $mech = WWW::Mechanize -> new();
$mech -> cookie_jar(HTTP::Cookies->new);

undef %done;

$user = '';
$pass = '';

$url = 'https://www.securecoding.cert.org/confluence/login.action';
$mech -> get($url);
$mech -> form_name('loginform');
$mech -> field('os_username', $user);
$mech -> field('os_password', $pass);
$mech -> submit();

#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/CERT+C+Programming+Language+Secure+Coding+Standard';

#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/01.+Preprocessor+%28PRE%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/02.+Declarations+and+Initialization+%28DCL%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/03.+Expressions+%28EXP%29';
$url = 'https://www.securecoding.cert.org/confluence/display/seccode/04.+Integers+%28INT%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/05.+Floating+Point+%28FLP%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/06.+Arrays+%28ARR%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/07.+Characters+and+Strings+%28STR%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/08.+Memory+Management+%28MEM%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/09.+Input+Output+%28FIO%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/10.+Environment+%28ENV%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/11.+Signals+%28SIG%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/12.+Error+Handling+%28ERR%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/13.+Miscellaneous+%28MSC%29';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/50.+POSIX+%28POS%29';


open(ALEX, ">alex2.txt");

$mech -> get($url);
my @links = $mech -> links;
my @links_new = ();
my $first = 1;
my $max = 9999;
my $count = 0;

while ($first || $#links_new > 0 && $count < $max) {
  $count++;
  
  $first = 0;
  @links_new = ();
  
  foreach my $link (@links) {
    next if($link == NULL);

    my $href = $link -> url;
    my $name = $link -> text;

    next if ($href !~ /confluence\/display\/seccode\//);
    if ($done{$href} == 1 || $href =~ /#/) {
      next;
    } else {
      $done{$href} = 1;
    }

    $mech -> get($href);
    next unless($mech -> success());
      
    $content = $mech -> content; 

    if ($mech -> title =~ /^[a-zA-Z0-9]{5}-.\./) {
      $name = substr($mech -> title, 0, 7);

      print("Found name: $name\n");

      $content =~ /<span id="labelsList">(.*?)<\/span>/s;
      $labels = $1;

      print ALEX $mech -> title;     

      if($labels =~ /unenforceable/) {
	print ALEX ',n/a,n/a,n/a,n/a,n/a,n/a,n/a';
      }
      else {
	$found = ($content =~ /<tt>(.*?)<\/tt> flag is used/);
	$flag = $1;
	$flag = "n/a" if($found ne "1");

	$rose = "EDG" if($labels =~ /rose-gcc/);
	$rose = "partial" if($labels =~ /rose-partial/);
	$rose = "complete" if($labels =~ /rose-complete/);
	$rose = "n/a" if($rose eq "");

	$ldra = "yes" if($content =~ /LDRA/);
	$ldra = "n/a" if($ldra eq "");

	$fortify = "yes" if($content =~ /fortify/);
	$fortify = "n/a" if($fortify eq "");

	$found = ($content =~ /<b>(.*?)<\/b> checker/);
	$coverity = $1;
	$coverity = "n/a" if($found ne "1");

	$splint = "yes" if($content =~ /Splint/);
	$splint = "n/a" if($splint eq "");

	$klocwork = "n/a" if($klocwork eq "");

	print ALEX ",$flag,$rose,$ldra,$fortify,$coverity,$splint,$klockwork";
      }

      print ALEX "\n";

      next;  	
    }

    $mech -> get($href.'?showChildren=true');
    next unless($mech -> success());
    @links_new = (@links_new, $mech -> links);
    @links = @links_new;
  }
}

close(ALEX);
print("\n");
