#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>


//change this value to change the grid-size / sfml window resolution
#define resolution 16384


using namespace std;
using namespace sf;

int main()
{
    //used to calculate the scrolling offset
    bool drag=false;
    Glsl::Vec2 current_offset;
    Glsl::Vec2 dragOrigin;

    //we want to count the current generation...
    long long generation=0;

    float zoom=1.0;
    Clock uptime;
    Event event;

    //create the renderwindow and enable VSync. Else the thing will reach
    //blazing high FPS (10000+) at low resolutions
    RenderWindow *window=new RenderWindow(VideoMode(resolution,resolution),"Game of Life GLSL");
    window->setVerticalSyncEnabled(true);
    //    window->setFramerateLimit(5);

    //we use a simple Quad as canvas, that fills the whole window
    VertexArray canvas(Quads,4);
    canvas[0]=Vertex(Vector2f(0,0),Color::White,Vector2f(0,0));
    canvas[1]=Vertex(Vector2f(0,resolution),Color::White,Vector2f(0,resolution));
    canvas[2]=Vertex(Vector2f(resolution,resolution),Color::White,Vector2f(resolution,resolution));
    canvas[3]=Vertex(Vector2f(resolution,0),Color::White,Vector2f(resolution,0));


    //we need two Rendertextures to hold the current and the coming generation
    //Also we enable Repeated, so we have an "infinite" Grid
    RenderTexture *nextGen = new RenderTexture;
    nextGen->create(resolution,resolution,ContextSettings());
    nextGen->setRepeated(true);

    RenderTexture *currentGen = new RenderTexture;
    currentGen->create(resolution,resolution,ContextSettings());
    currentGen->setRepeated(true);

    //Init the current generation with a randowm pattern.
    //We can't access the RenderTextures Directly from here.
    //So let's create a new sf::Image first and fill it with data.
    Image *initState=new Image;
    initState->create(resolution,resolution);
    for(uint y=0;y<resolution;y++)
        for(uint x=0;x<resolution;x++)
        {
            unsigned char color=(rand()%2)*255;
            initState->setPixel(x,y,Color(color,color,color,255));
        }

    //The create a texture and copy the Image there. After this the texture is rendered to
    //the RenderTexture
    Texture *upload=new Texture;
    upload->loadFromImage(*initState);
    currentGen->draw(canvas,upload);
    currentGen->display();

    //We don't need the initial sf::Image and texture anymore. So we free up the memory.
    delete upload;
    delete initState;



    //load and compile neccesary shaders, also init the uniforms in the shaders
    Shader shader;
    shader.loadFromFile("vertex.glsl","fragment.glsl");
    shader.setUniform("resolution",float(resolution));

    Shader dispShader;
    dispShader.loadFromFile("vertex.glsl","display_fragment.glsl");
    dispShader.setUniform("zoom",zoom);

    //create some states, so SFML can bind shaders and Texture to OpenGL
    RenderStates states;
    states.shader=&shader;
    states.texture=&currentGen->getTexture();

    RenderStates dispStates;
    dispStates.shader=&dispShader;

    //Let's have some text on screen. The font and everythig is defined here
    Font font;
    Text text;

    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setOutlineColor(Color::Blue);
    text.setOutlineThickness(resolution/400.0f);
    text.setCharacterSize(int(resolution/50.0));
    text.setPosition(resolution/50.0f,resolution/50.0f);

    //Used for measure the framerate
    float start=0;
    float end=0;

    while(window->isOpen())
    {

        //Process window events.
        //You can adjust the zoom by the Mousewheel or scroll around while pressing
        //the left mousebutton
        while (window->pollEvent(event))
        {
            switch(event.type)
            {
            case Event::Closed:
                window->close();
                break;
            case Event::MouseWheelScrolled:
                zoom+=event.mouseWheelScroll.delta*.1;
                zoom=zoom<.25f?.25f:zoom;
                dispShader.setUniform("zoom",zoom);
                break;
            case Event::MouseButtonPressed:
                if(event.mouseButton.button==Mouse::Left)
                {
                    drag=true;
                    dragOrigin=Glsl::Vec2(Mouse::getPosition());
                }
                break;
            case Event::MouseButtonReleased:
                if(event.mouseButton.button==Mouse::Left)
                {
                    drag=false;
                    dragOrigin=Glsl::Vec2(0,0);
                }
                break;
            default:
                break;

            }
        }


        //"Draw" the current generation to the next generation.
        //The Game of Life simulation is happening here.
        nextGen->draw(canvas,states);
        nextGen->display();

        //Now swap the pointers to the Renderbuffers, so we do not need to copy any
        //texture data around.
        RenderTexture *swap=currentGen;
        currentGen=nextGen;
        nextGen=swap;

        //if the left mousebutton is pressed, we need to calculate the scrolling offset
        if(drag)
        {
            Glsl::Vec2 offset;
            offset=Glsl::Vec2(Mouse::getPosition());
            offset=dragOrigin-offset;
            offset.y*=-1;
            offset.x/=resolution;
            offset.y/=resolution;
            current_offset+=offset;
            dragOrigin=Glsl::Vec2(Mouse::getPosition());
        }
        //Prepare the textures and uniforms, so they can be drawn to our window
        dispStates.texture=&currentGen->getTexture();
        dispShader.setUniform("offset",current_offset);


        //Now we can draw everything
        window->draw(canvas,dispStates);
        window->draw(text);
        window->display();


        //Add our text and update the frametimes
        end=uptime.getElapsedTime().asSeconds();

        stringstream stringBuffer;
        stringBuffer << (1.0f/(end-start)) << "\n";
        stringBuffer << "Grid: " << resolution << "x" << resolution << "\n";
        stringBuffer << "Cells: " << resolution * resolution << "\n";
        stringBuffer << "Generation: " << generation << "\n";
        stringBuffer << "Time: " << uptime.getElapsedTime().asSeconds() << "\n";
        stringBuffer << "Zoom: " << zoom << "\n";
        text.setString(stringBuffer.str());
        start=uptime.getElapsedTime().asSeconds();
        generation++;
    }

    //Clean up after yourself. ;)
    delete currentGen;
    delete nextGen;
    delete window;

    return 0;
}
