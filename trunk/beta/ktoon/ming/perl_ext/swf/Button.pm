# ====================================================================
# Copyright (c) 2000 by Soheil Seyfaie. All rights reserved.
# This program is free software; you can redistribute it and/or modify
# it under the same terms as Perl itself.
# ====================================================================

# $Author: soheil $
# $Id: Button.pm,v 1.2 2001/09/24 00:53:00 soheil Exp $

package SWF::Button;
use SWF();

$SWF::Button::VERSION = $SWF::VERSION;

1;

__END__

=head1 NAME

SWF::Button - SWF button class

=head1 SYNOPSIS

 use SWF::Button;


=head1 DESCRIPTION

Creates buttons for flash movies.

=head1 METHODS

=over 4

=item new SWF::Button();

Creates a new Button object.

=item $button->addShape($shape, FLAG);

Adds $shape to the button. Valid FLAGs are:

    SWFBUTTON_HIT
    SWFBUTTON_UP
    SWFBUTTON_DOWN
    SWFBUTTON_OVER


=item $button->setOver($shape);

Shortcut for $button->addShape($shape, SWFBUTTON_OVER);

=item $button->setHit($shape);

Shortcut for $button->addShape($shape, SWFBUTTON_HIT);

=item $button->setUp($shape);

Shortcut for $button->addShape($shape, SWFBUTTON_UP);

=item $button->setDown($shape);

Shortcut for $button->addShape($shape, SWFBUTTON_DOWN);


=item $button->setAction($action [,FLAG]);

Adds $action object (see SWF::Action). Valid FLAGs are:

    SWFBUTTON_MOUSEUP
    SWFBUTTON_MOUSEOVER
    SWFBUTTON_MOUSEOUT
    SWFBUTTON_MOUSEDOWN
    SWFBUTTON_DRAGOVER
    SWFBUTTON_DRAGOUT

SWFBUTTON_MOUSEUP is the default value of FLAG.

=back

=head1 AUTHOR

Soheil Seyfaie (soheil@netcom.ca).

=head1 SEE ALSO

SWF, SWF::Action, SWF::Shape, SWF::Movie, .


=cut
