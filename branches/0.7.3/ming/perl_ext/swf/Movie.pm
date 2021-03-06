# ====================================================================
# Copyright (c) 2000-2003 by Soheil Seyfaie. All rights reserved.
# This program is free software; you can redistribute it and/or modify
# it under the same terms as Perl itself.
# ====================================================================

# $Id: Movie.pm,v 1.4 2003/01/21 00:23:01 soheil Exp $

package SWF::Movie;
use SWF();

$SWF::Movie::VERSION = $SWF::VERSION;

use strict;

sub streamMp3{
    my ($movie,$filename) = @_;
    $movie->setSoundStream(new SWF::Sound($filename));
}

sub output{
    #/xxx/ Should we allow use to specify???
    my $output_method = \&xs_output;
    $output_method = \&Apache::SWF::xs_output
	if ( $INC{'Apache/SWF.pm'} && 
	     substr($ENV{GATEWAY_INTERFACE}||'',0,8) eq 'CGI-Perl');
    return &$output_method(@_);
}


1;
__END__

=head1 NAME

	SWF::Movie - SWF Movie class

=head1 SYNOPSIS

	use SWF::Movie;

	$m = new SWF::Movie();
	$m = new SWF::MovieWithVersion(version);

=head1 DESCRIPTION

SWF::Movie is the root object of your flashfile. It contains all your other objects you want in your movie.
Ming supports many Flash4 and Flash5 features and some of Flash MX(Flash6).

The SWF::Movie object contains information like the flashversion, the size, the frames per second, the background color. Except that, the SWF::Movie behaves like 
a SWF::MovieClip. It only cannot be added to another SWF::Movie or SWF::MovieClip. ;-)

Notice: The version of your flashmovie can be set by the object SWF. See SWF for more details.

=head1 METHODS

=over

=item add(SWFObject)

Add an instance of a SWF-object to your current frame. The returnvalue is a SWF::DisplayItem.

	Possible SWF-objects are:
 	
	SWF::Action
	SWF::MovieClip
	SWF::Shape
	SWF::Sound
	SWF::Text
	SWF::TextField
	
	$m->add($shape); # possible, but not more accessible for us
	$shape_1 = m->add($shape); # good instance 

=item addExport(SWF::Block, name)

??? what this method is good for ???

=item remove(SWF::DisplayItem)

Removes the given instance from the current and following frames of the main timeline. 

	$m->remove($shape_i);

=item nextFrame()

Finish a frame of the main timeline. New  added objects will be added in the next frame.

	$m->nextFrame();

=item setFrames(int)

Preset the number of frames of the main timeline. n not used frames before finishing the movie will filled with n nextFrame() statements.
If you inserted more nextFrame() commands than you set with setFrames() the setFrames() will be ignored. 

	$m->setFrames(100);

=item setBackground(red,green,blue)

Set the background in red, green, blue values. Possible value are 0..255 or hexvalues 0x0..0xff.

	$m-setBackground(0xff,0x99,0x00); # orange bg
	$m-setBackground(0,200,255)       # green bg

A transparency level isn't available I asume. (In Flash MX/F6?). A transparent background can be set in the HTML-code for Internet Explorer. Please 
inform the Authors if that is available for nonIE-browsers.

=item setDimension(x,y)

Set the dimension in pixel.

	#We should implement this in ming c-lib!!
	#min: (0.0,0.0)   # ;-)
	#max: (?,?)
	#default: (0.0?,0.0?)
	#Behavior when values <min: ming sets it to min
	#Behavior when values >max: ming sets it to max
	
	$m->setDimension(200,100);
	$m->setDimension(123.45,67.89);

=item setRate(int)

Set the playing speed in frames/second. 

	#We should implement this in ming c-lib!!
	#min: 0
	#max: ?
	#default: ?
	#Behavior when value <min: ming sets it to min
	#Behavior when value >max: ming sets it to max
	
	$m->setRate(30);

=item protect()

A feature that prevents only flashnewbies from importing your movie into Macromedias Flash authoring tools. It cannot protect your content from being loaded by 
other flashmovies, deleting of the protection with a hexeditor or disassembling with tools. 

	$m->protect();

=item setSoundStream(SWF::Sound)

Puts an SWF::Sound object as streaming sound into the main timeline.

	$m->setSoundStream($sound);

=item streamMp3(filename)

Simplifies the setSoundStream(). You only need the filename. 

	$m->("iloveyou.mp3");

Hint: If you want to reuse this sound/mp3 later in this movie, create a reusable SWF::Sound object and then put it into setSoundstream().

=item labelFrame(string)

Gives the current frame a label that can be used by actionscript as anchor. Then you don't have to count your nextFrame() lines anymore. :-)

	$m->labelFrame("win");

	#max characters of a label??? 
	#What happen if we put there a big string?? 
	#Which characters are allowed??

=item output([int compresslevel])

Writes the SWF::Movie to stdout. This is normally used for webservers/cgi-scripts. 
Don't forget to send the "Content-Type:"-lines before writing out the movie.

The parameter compresslevel is optional. compresslevel between 0 and 9. Higher values will be interpreted as 9. Values smaller 0
mean no compression. By default the SWF::Movie will be send without compression. 

Notice: compression available since Flash MX(Flash 6). Ming does not check this for you if you write flash4 or flash5 movies.

=item save(filename [, int compresslevel]) 
	
Save the SWF::Movie to a file. 

The parameter compresslevel is optional. compresslevel between 0 and 9. Higher values will be interpreted as 9. Values smaller 0 
mean no compression. By default the SWF::Movie will be saved without compression. 

Notice: compression available since Flash MX(Flash 6). Ming does not check this for you if you write flash4 or flash5 movies.

=back

=head1 EXAMPLE

3 examples: a regular minimal SWF::Movie, one with all methods and a more interesting.

=over

=item minimal empty example

	#!/usr/bin/perl -w
	use SWF::Movie;
	
	$m = new SWF::Movie();
	$m->nextFrame();
	$m->save("emptyminimal.swf");

=item full empty examples

	#!/usr/bin/perl -w
	use SWF::Movie;
	
	$m = new SWF::Movie();
	$m->setVersion(4);
	$m->setBackground(0xff,0xcc,0x0);
	$m->setDimension(200,100);
	$m->protect();
	$m->setRate(30);
	$m->setFrames(10); # ming fills it automatic with frames
	
	$m->nextFrame()
	$m->save("empty.swf");

=item streaming example

	#!/usr/bin/perl -w
	
	use SWF("Shape");
	SWF::setVersion(6);
	
	$m = new SWF::Movie();
	$m->setBackground(0xff,0xcc,0x0);
	$m->setDimension(200,100);
	$m->setFrames(10);
	$m->streamMp3("forever.mp3");
	
	$s= new SWF::Shape();
	$s->setLine(40,0xff,0xff,0);
	$s->setRightFill(0xff,0,0); 
	$s->drawLine(50,0);
	$s->drawLine(0,20);
	$s->drawLineTo(0,0);
	
	$s_1= $m->add($s);
	$s_2= $m->add($s);
	$s_1->move(50,40);
	$s_2->move(10,40);
	for($i=1;$i<5;$i++) {
		$m->nextFrame();
		$s_2->move(0,10);
	}
	$m->nextFrame();
	print "Content-type: application/x-shockwave-flash\n\n";

	$m->output(9); # with compression level 9 (since flash6)

=back

=head1 AUTHOR

Soheil Seyfaie (soheil at users.sourceforge.net).

=head1 SEE ALSO

SWF, SWF::Action, SWF::Bitmap, SWF::Button, SWF::Constants, SWF::DisplayItem, SWF::Fill, SWF::Font, SWF::Gradient, SWF::Morph, SWF::Movie, SWF::Shape, SWF::Sound, SWF::Sprite, SWF::TextField, SWF::Text

=cut
