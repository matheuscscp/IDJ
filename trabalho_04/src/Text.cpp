#include "simplestructures.hpp"

#include "Text.hpp"

using std::string;

Text::Text(
	const string& fontfile,
	const string& text,
	int ptsize,
	int style,
	const SDL_Color& color,
	render_types render_type,
	const SDL_Color& bg
) : font(NULL), surface(NULL), fontfile(fontfile), text(text), style(style),
color(color), render_type(render_type), bg(bg)
{
	font = TTF_OpenFont( fontfile.c_str(), ptsize );
	if( !font )
		throw( mexception( "TTF_OpenFont error" ) );
	TTF_SetFontStyle( font, style );
	
	render_text_();
}

Text::~Text()
{
	TTF_CloseFont( font );
	SDL_FreeSurface( surface );
}

int Text::w() const
{
	return surface->w;
}

int Text::h() const
{
	return surface->h;
}

void Text::render (int x, int y)
{
	SDL_Rect dstrect;
	
	dstrect.x = x;
	dstrect.y = y;
	
	SDLBase::renderSurface ( surface, NULL, &dstrect );
}

void Text::render_text()
{
	SDL_FreeSurface( surface );
	render_text_();
}

void Text::render_text_()
{
	switch( render_type )
	{
	case solid:
		surface = TTF_RenderUTF8_Solid( font, text.c_str(), color );
		break;
		
	case blended:
		surface = TTF_RenderUTF8_Blended( font, text.c_str(), color );
		break;
		
	case shaded:
		surface = TTF_RenderUTF8_Shaded( font, text.c_str(), color, bg );
		break;
		
	default:
		break;
	}
}

void Text::setStyle(int style)
{
	this->style = style;
	TTF_SetFontStyle( font, style );
	
	render_text();
}

void Text::setSize(int ptsize)
{
	TTF_CloseFont( font );
	font = TTF_OpenFont( fontfile.c_str(), ptsize );
	TTF_SetFontStyle( font, style );
	
	render_text();
}

void Text::setText(const string& text)
{
	this->text = text;
	render_text();
}

void Text::setColor(const SDL_Color& color)
{
	this->color = color;
	render_text();
}

void Text::setRenderType(render_types render_type)
{
	this->render_type = render_type;
	render_text();
}

void Text::setBGColor(const SDL_Color& bg)
{
	this->bg = bg;
	render_text();
}
