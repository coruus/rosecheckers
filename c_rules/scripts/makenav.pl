#!/usr/bin/perl -w

BEGIN {
  unshift(@INC, "/afs/andrew.cmu.edu/usr16/jpincar/cert/WWW-Mechanize-1.30/lib/");
}

use WWW::Mechanize;
my $mech = WWW::Mechanize -> new();
$mech -> cookie_jar(HTTP::Cookies->new);

$user = '';
$pass = '';

doMechLogin();

# CERT C++ Secure Coding Standard
$space = 'CERT C++ Secure Coding Standard';
$topURL = 'https://www.securecoding.cert.org/confluence/pages/viewpage.action?pageId=637';
$baseURL = 'https://www.securecoding.cert.org/confluence/display/cplusplus/';

$mech -> get($topURL);

my @sections = ();
$content = $mech->content;

while($content =~ /<a href="\/confluence\/display\/\w+?\/\d\d\.[a-zA-Z0-9 _+\(\)%]+?" title="(.*?)">/sg) {
  push @sections, $1;
}

$, = "\n";

$prev = $space;

$counterS = 0;

foreach $section (@sections) {
  $counterS++;
  $up = $space;

  print("Spidering through: $section\n");

  $mech->get($baseURL.$section);
  do{print("Could not get: $baseURL$section\n"); next;} unless($mech -> success());

  @articles = ();

  $content = $mech->content;

  $content =~ /<!-- wiki content -->(.*?)(?:<hr|<div id="pageChildren" class="pageSection")/s;
  $content = $1;

  while($content =~ /<a href="\/confluence\/display\/\w+?\/[a-zA-Z0-9]{5}-.+?" title="(.*?)">/sg) {
#  while($content =~ /<a href="\/confluence\/display\/\w+?\/[\-\.a-zA-Z0-9 _+\(\)%]+?" title="(.*?)">/sg) {
    push @articles, $1;
  }
  
  $next = $articles[0];

  addNav();
  print("\tAdded navigation to page: $section\n");

  $prev = $section;
  $up = $section;

  $counterA = 0;

  foreach $article (@articles) {
    $counterA++;

    $next = $#articles >= $counterA ? $articles[$counterA] : $sections[$counterS];

    $mech->get($baseURL.$article);
    addNav();
    print("\tAdded navigation to page: $article\n");

    $prev = $article;
  }
}

die("\nComplete!\n\n");


######################################
## Code ends here. Only subs below. ##
######################################

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

sub addNav {
  if($mech->content =~ /"(\/confluence\/pages\/editpage.action\?pageId=\d+)"/) {
    $fullURL = 'https://www.securecoding.cert.org'.$1;

    $mech->get($fullURL);

    do{print("Could not edit page: $fullURL"); return;} unless($mech -> success());
    
    my $content = $mech -> content; 
    $content =~ /name="content".*?>(.*?)<\/textarea>/s;
    $content = $1;

    $content =~ s/&amp;/&/g;
    $content =~ s/&quot;/"/g;
    $content =~ s/&lt;/</g;
    $content =~ s/&gt;/>/g;

    #Actually add/change nav here

    if($content =~ /----/)  {
      $content =~ s/\s*----.*$//s;
    }

#    print "Adding Nav:\n\tPrev: $prev\n\tUp: $up\n\tNext: $next\n\t\n";

    $newNav = "----\n[!$space^button_arrow_left.png!|$prev]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[!$space^button_arrow_up.png!|$up]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[!$space^button_arrow_right.png!|$next]\n\n";

    $content .= "\n\n".$newNav;

    $comment = "Edited by NavBot (jp)";

     $mech -> form_name('editpageform'); 
     $mech -> field('content', $content);
     $mech -> field('versionComment', $comment);
     $mech -> click('confirm');
  }
  else {
    print("Could not find edit page: ".$mech->uri().".\n");
  }
}
