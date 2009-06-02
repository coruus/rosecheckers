#!/usr/bin/perl

BEGIN {
  unshift(@INC, "/afs/andrew.cmu.edu/usr16/jpincar/cert/WWW-Mechanize-1.30/lib/");
}

$version = "v3.1";

use WWW::Mechanize;
my $mech = WWW::Mechanize -> new();
$mech -> cookie_jar(HTTP::Cookies->new);

undef %done;

$user = '';
$pass = '';

doMechLogin();

 $url = 'https://www.securecoding.cert.org/confluence/display/java/00.+Security+(SEC)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/01.+Declarations+and+Initialization+(DCL)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/02.+Expressions+(EXP)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/03.+Scope+(SCP)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/04.+Integers+(INT)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/05.+Floating+Point+(FLP)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/06.+Objects+Orientation+(OBJ)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/07.+Input+Output+(FIO)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/08.+Concurrency+(CON)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/09.+Methods+(MET)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/10.+Exceptional+Behavior+(EXC)';
# $url = 'https://www.securecoding.cert.org/confluence/display/java/11.+Miscellaneous+(MSC)';

$url .= "?showChildren=true";
$url =~ /\((.*?)\)/;
$section = $1;

die("Could not detect section.\n") if($section eq "");
print("Spidering through ($section) section.\n");

$mech -> get($url);
my @links = $mech -> links;
my $maxDepth = 1;
my $depth = 0;

@hrefs = ();

foreach $link (@links) {
  next if($link == NULL);
  unshift(@hrefs, $link -> url);
}

@hrefs = sort(@hrefs);

$riskblock = "";

open(RISKS, ">risks.txt");

while($depth < $maxDepth) {
  foreach my $href (@hrefs) {
    next if($href eq "");

    if($done{$href} == 1 || $href =~ /#/ || $href !~ /$section/) {next;}
    else {$done{$href} = 1;}

    if($href =~ /[a-zA-Z0-9]{5}-/) {
      $mech -> get($href);
      next unless($mech -> success());
   
      print("href: $href\n");

      $content = $mech -> content; 

      $content =~ /<td.*?>\s*([a-zA-Z0-9]{5}-.{1,3})\s*<\/td>.*?<td.*?>\s*([[:alpha:]]*)\s*<\/td>.*?<td.*?>\s*([[:alpha:]]*)\s*<\/td>.*?<td.*?>\s*([[:alpha:]]*)\s*<\/td>/s;

print("1: $1\n");

      print RISKS checkrisk($1, $2, $3, $4)."\n";

#       $comment = "Edited by sciSpider CPP $version (sch jp)"; # (X_X)@==(Q_Q)@";

#       $mech -> form_name('editpageform'); 
#       $mech -> field('content', $input);
#       $mech -> field('versionComment', $comment);
#       $mech -> click('confirm');
      print("\tArticle complete.\n");
      
      next;  
    }
    
    next if ($href !~ /confluence\/display\/java\//);
  
    $mech -> get($href.'?showChildren=true');
    next unless($mech -> success());
  }

  $depth++;
}

close(RISKS);
die("\nSpidering complete.\n\n");

######################################
## Code ends here. Only subs below. ##
######################################

sub checkrisk {
  my $rule = shift;
  my $severity = shift;
  my $likelihood = shift;
  my $cost = shift;

  if ($severity eq "high") {
    $sev = 3;
  } elsif ($severity eq "medium") {
    $sev = 2;
  } elsif ($severity eq "low") {
    $sev = 1;
  } else {
    $sev = 0;
  }

  if ($likelihood eq "likely") {
    $lik = 3;
  } elsif ($likelihood eq "probable") {
    $lik = 2;
  } elsif ($likelihood eq "unlikely") {
    $lik = 1;
  } else {
    $lik = 0;
  }

  if ($cost eq "high") {
    $cos = 1;
  } elsif ($cost eq "medium") {
    $cos = 2;
  } elsif ($cost eq "low") {
    $cos = 3;
  } else {
    $cos = 0;
  }

  $total = $sev * $lik * $cos;

  if ($total > 11) {
    $end = "{color:red}{*}P".$total."{*}{color} | {color:red}{*}L1{*}{color}"; 
  } elsif ($total > 5) {
    $end = "{color:#cc9900}{*}P".$total."{*}{color} | {color:#cc9900}{*}L2{*}{color}"; 
  } elsif ($total > 0) {
    $end = "{color:green}{*}P".$total."{*}{color} | {color:green}{*}L3{*}{color}"; 
  } else {
    $end = "{color:blue}{*}P???{*}{color} | {color:blue}{*}L???{*}{color}";
  }

  return "| $rule | $severity | $likelihood | $cost | $end |";
}

sub savecode {
  my $ncce = '';

  while ($input =~ /\{code\:bgColor\s*=\s*\#FFCCCC.*?\}(.*?)\{code\}/gsi) {
    $ncce .= "\n/* Begin {code} */\n".$1."\n/* End {code} */\n";
  }

  open(NCCE, ">code/".$name."_ncce.cpp");
  print NCCE $ncce;
  close(NCCE);

  my $cs = '';

  while ($input =~ /\{code\:bgColor\s*=\s*\#CCCCFF.*?\}(.*?)\{code\}/gsi) {
    $cs .= "\n/* Begin {code} */\n".$1."\n/* End {code} */\n";
  }
	  
  open(CS, ">code/".$name."_cs.cpp");
  print CS $cs;
  close(CS);
}

sub fixHtmlCodes {
  $input =~ s/&amp;/&/g;
  $input =~ s/&quot;/"/g;
  $input =~ s/&lt;/</g;
  $input =~ s/&gt;/>/g;
  # $input =~ s/&nbsp;/ /g;
}

sub doEditorChanges {
  $input =~ s/non-compliant/noncompliant/g;
  $input =~ s/Non-Compliant/Noncompliant/g;
  $input =~ s/Non-compliant/Noncompliant/g;
  $input =~ s/NCCE/NCE/g;

  $input =~ s/int main\(\)/int main\(void\)/g;

  #      $input =~ s/NULL-termination/null-termination/g;
  #      $input =~ s/NULL-terminated/null-terminated/g;
  $input =~ s/The tool Compass Rose/Compass\/ROSE/g;
  $input =~ s/Compass \/ Rose/Compass\/ROSE/g;
  $input =~ s/\\\[\[ISO\/IEC 9899-1999\|AA\. C References\#ISO\/IEC 9899-1999\]\\\]/\\\[\[ISO\/IEC 9899:1999\|AA\. C References\#ISO\/IEC 9899-1999\]\\\]/g;
  $input =~ s/compilant/compliant/g;
  $input =~ s/Compilant/Compliant/g;
  $input =~ s/Unix/UNIX/g;
  $input =~ s/denial of service attack/denial-of-service attack/g;
  $input =~ s/denial of service condition/denial-of-service attack/g;
  $input =~ s/14882-2003\|/14882\:2003\|/g;
  $input =~ s/24731-1-2007\|/24731-1\:2007\|/g;
  $input =~ s/in order to/to/g;
  $input =~ s/in order for/for/g;
  $input =~ s/Therefore/Consequently/g;
  $input =~ s/therefore/consequently/g;
  $input =~ s/Thus/As a result/g;
  $input =~ s/thus/as a result/g;
  $input =~ s/utilized/used/g;
  #      $input =~ s/NULL pointer/null pointer/g;
  #      $input =~ s/NULL character/null character/g;
  $input =~ s/typecast/type cast/g;
  $input =~ s/typecasts/type casts/g;
  $input =~ s/illustrates/shows/g;
  $input =~ s/Posix/POSIX/g;
  $input =~ s/run-time/runtime/g;
  $input =~ s/Shell code/Shellcode/g;
  $input =~ s/shell code/shellcode/g;
  $input =~ s/is able to/can/g;
  $input =~ s/h2\. Automated/h3\. Automated/;

  # These might break things in code
  #      $input =~ s/filename/file name/g;
  #      $input =~ s/Filename/File name/g;
  $input =~ s/filesystem/file system/g;
  $input =~ s/Filesystem/File System/g;
  #      $input =~ s/pathname/path name/g;
  #      $input =~ s/Pathname/Path name/g;
}

sub doMechLogin {
  my $loginUrl = 'https://www.securecoding.cert.org/confluence/login.action';
  $mech -> get($loginUrl);
  $mech -> form_name('loginform');
  $mech -> field('os_username', $user);
  $mech -> field('os_password', $pass);
  $mech -> submit();
  die("Unable to login.\n") if (!$mech->success());
  die("\nSorry, your username and password are incorrect - please try again.\n\n") if ($mech->content() =~ /Sorry, your username and password are incorrect - please try again./);
}

sub doShaunChanges {
  open(FILE, ">content.txt") or die("Cannot open file for writing\n");
  print FILE ($mech -> title)."\n";
  print FILE $text;
  close(FILE);
  
  $input = `./a.out`;
  
  undef $success;
  open(SUCCESS, "success.txt") or die("Cannot open file for writing\n");
  $success = <SUCCESS>;
  close(SUCCESS);
  
  if($success !~ /\^_+\^/) {
    print("Warning: External changes did not succeed.\n");
    $input = $text;
  }
}

sub doRelatedLink {
  my $newtext = "Search for vulnerabilities resulting from the violation of this rule on the [CERT website|https://www.kb.cert.org/vulnotes/bymetric?searchview&query=FIELD+KEYWORDS+contains+$name"."-CPP].";

  if ($input =~ /Examples of vulnerabilities resulting from the.*?\]\.?/s) {
    $input =~ s/Examples of vulnerabilities resulting from the.*?\]\.?/h3. Related Vulnerabilities\n\n$newtext/s;
  } elsif ($input =~ /Search for .*? resulting from .*?\]\.?/s) {
    $input =~ s/Search for .*? resulting from .*?\]\.?/$newtext/s;
  } else {
    $input =~ s/(h2\. Risk Assessment.*?color.*?color.*?color.*?color.*?.*?\|)\s*/$1\n\nh3. Related Vulnerabilities\n\n$newtext\n\n/s;
  }
}
