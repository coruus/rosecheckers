#!/usr/bin/perl

#PRAGMA AI=ON

BEGIN {
  unshift(@INC, "/afs/andrew.cmu.edu/usr16/jpincar/cert/WWW-Mechanize-1.30/lib/");
}

use WWW::Mechanize;
my $mech = WWW::Mechanize -> new();
$mech -> cookie_jar(HTTP::Cookies->new);

undef %done;

$user = '';
$pass = '';

doMechLogin();

#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/01.+Preprocessor+(PRE)';
$url = 'https://www.securecoding.cert.org/confluence/display/seccode/02.+Declarations+and+Initialization+(DCL)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/03.+Expressions+(EXP)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/04.+Integers+(INT)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/05.+Floating+Point+(FLP)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/06.+Arrays+(ARR)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/07.+Characters+and+Strings+(STR)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/08.+Memory+Management+(MEM)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/09.+Input+Output+(FIO)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/10.+Environment+(ENV)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/11.+Signals+(SIG)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/12.+Error+Handling+(ERR)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/13.+Application+Programming+Interfaces+(API)
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/49.+Miscellaneous+(MSC)';
#$url = 'https://www.securecoding.cert.org/confluence/display/seccode/50.+POSIX+(POS)';

$url .= "?showChildren=true";
$url =~ /\((.*?)\)/;
$section = $1;

die("Could not detect section.\n") if($section eq "");
print("Spidering through ($section) section.\n");

$mech -> get($url);
my @links = $mech -> links;
my $maxDepth = 2;
my $depth = 0;

while($depth < $maxDepth) {
  @links_new = ();
  
  foreach my $link (@links) {
    next if($link == NULL);

    my $href = $link -> url;

    if($done{$href} == 1 || $href =~ /#/ || $href !~ /$section|editpage/) {next;}
    else {$done{$href} = 1;}

    if($href =~ /confluence\/pages\/editpage.action\?pageId=3693/) {
      $mech -> get($href);
      next unless($mech -> success());

      $mech -> content =~ /<input type="text" name="title" size="43" value="(.*?)"/;
      $title = $1;

      $content = $mech -> content; 

      $content =~ /<div id="labels_info">(.*?)<\/div>/s;
      $labels = $1;
      $labels =~ s/\n//g;

      $content =~ /name="content".*?>(.*?)<\/textarea>/s;
      $content = $1;

      if($title =~ /^([a-zA-Z0-9]{5})-/) {
	$name = $1;
	print("This is C rule: $name\n");

	doEditorChanges();
	doRelated();
	doAutomated();
#	doOrder();
      }
      else {
	next;
      }

      $comment = "Edited by sciSpider";

      $title =~ s/([a-zA-Z0-9]{5}-)./$1C/;

      $mech -> form_name('editpageform'); 
      $mech -> field('title', $title);
      $mech -> field('content', $content);
      $mech -> field('versionComment', $comment);
      $mech -> click('confirm');
      print("\t\tEdit complete.\n\n");

      die;
      next;  
    }
    
    next if ($href !~ /confluence\/display\/seccode\//);
  
    $mech -> get($href.'?showChildren=true');
    next unless($mech -> success());
    @links_new = (@links_new, $mech -> links);
  }
  
  @links = @links_new;
  $depth++;
}

die("\nSpidering complete.\n\n");

######################################
## Code ends here. Only subs below. ##
######################################

sub doEditorChanges {
  $content =~ s/h.\. References/h2\. Bibliography/;

  $content =~ s/&quot;/"/g;
  $content =~ s/&amp;/&/g;
  $content =~ s/&lt;/</g;
  $content =~ s/&gt;/>/g;

  $content =~ s/&quot;/"/g;
  $content =~ s/&amp;/&/g;
  $content =~ s/&lt;/</g;
  $content =~ s/&gt;/>/g;
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

sub doAutomated {

#  print("\tFound labels: ".$labels."\n");

  if($labels =~ /unenforceable/) {
    print("\tUnenforceable\n");
  }
  else {
    $block = "|| Tool || Compliance || Description ||\n";

    $rose = "EDG" if($labels =~ /rose-gcc/);
    $rose = "partial" if($labels =~ /rose-partial/);
    $rose = "complete" if($labels =~ /rose-complete/);
    
    if($rose ne "") {
      $content =~ s/^(.*?Compass\/ROSE.*?)$//i;
      $block .= "| Compass/ROSE | $rose | $1 |\n";
    }

    if($content =~ s/^(.*?Coverity.*?\*.*?\* checker.*?$)//i);
      $block .= "| Coverity | yes | $1 |\n";
    }

    if($content =~ s/^(.*?Fortify.*?)$//i) {
      $block .= "| Fortify | yes | $1 |\n";
    }

    $found = ($content =~ /\{\{(.*?)\}\} flag is used/);
    $flag = $1;
    $flag =~ s/\\-/-/g;
    $flag = "n/a" if($found ne "1");

    if($content =~ s/^(.*?Klocwork.*?\*.*?\* checker.*?$)//i);
      $block .= "| Klocwork | yes | $1 |\n";
    }

    if($content =~ s/^(.*?LDRA.*?)$//i) {
      $block .= "| LDRA | yes | $1 |\n";
    }

    if($content =~ s/^(.*?Splint.*?)$//i) {
      $block .= "| Splint | yes | $1 |\n";
    }

    if($content =~ /h.\. Auto(.*?)\n(.*?)(?=h\d\.|----)/s) {
      $rest = $2;
      $content =~ s/h.\. Auto.*?(h.\.)/h3. Automated Detection\n\n$block\n$rest$1/s
    }
  }
}

sub doRelated {
  if($content =~ /h3\. Other.*?\n(.*?)(?=h\d\.|----)/s) {
    $other = $1;
    
    $block = "|| Standard || Document ||\n";
    
    while($other =~ /(\[(?!c|j).*?\])/g) {
      $block .= "| CERT C | $1 |\n";
    }
    
    while($other =~ /(\[cplusplus:.*?\])/g) {
      $block .= "| CERT C\\+\\+ | $1 |\n";
    }

    while($other =~ /(\[java:.*?\])/g) {
      $block .= "| CERT Java | $1 |\n";
    }

    while($content =~ s/\\\[\[ISO\/IEC PDTR 24772\|AA\. C References\#ISO\/IEC PDTR 24772\]\\\] "(.*?)".*?\n//sgi) {
      $block .= "| ISO/IEC PDTR 24772 | $1 |\n";
    }
    
    if($content =~ s/\\\[\[MISRA.*?(\d+\.\d+).*?\n//sgi) {
      $block .= "| MISRA C: 2004| $1 |\n";
    }

    if($content =~ s/\\\[\[MITRE.*?CWE.*?(\d+).*?"(.*?)".*?\n//sgi) {
      $block .= "| MITRE CWE | ID $1, $2 |\n";
    }

    if($block eq "|| Standard || Document ||\n") {
      $block = "None yet.";
    }
   
   $content =~ s/h.\. Other.*?(?=h\d\.|----)/h3. Cross References\n\n$block/s
  }
}

sub doOrder() {
  if($content =~ /h\d\. Bib.*?(?=h\d\.|----)/si) {
    $content =~ s/h\d\. Bib.*?\n(.*?)(?=h\d\.|----)//si;
  $bibliography = $1;
  $content =~ s/----/h2. Bibliography\n$bibliography----/si;
  }
  else {
    return;
  }
  
if($content =~ /h\d\. Cross.*?(?=h\d\.)/si) {
  $content =~ s/h\d\. Cross.*?\n(.*?)(?=h\d\.)//si;
  $cross = $1;
  $content =~ s/(h2\. Rel)/h3. Cross References\n$cross$1/si;
}
else {
return;
}
}
