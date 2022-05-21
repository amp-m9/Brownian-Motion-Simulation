
// Created by andri on 08/02/2022.
//

#include "window_maker.h"
#include "resource_manager.h"
#include "RigidBody.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_glfw.cpp"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <filesystem>

const char *para1 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent ante velit, lobortis et diam eu, aliquam laoreet dui. Aliquam pharetra odio leo, id bibendum mi venenatis at. Sed non orci placerat, vulputate ex vitae, venenatis lectus. Praesent euismod, nisl eu cursus ornare, turpis lectus finibus dui, id laoreet risus est sit amet est. Sed nec sollicitudin ante. Maecenas porta, turpis sed placerat pretium, augue dolor egestas est, at hendrerit mauris purus ullamcorper nibh. Aliquam consectetur mattis velit nec porta. In vitae ante ac dui gravida dictum pulvinar eu quam. Maecenas facilisis sem in eros egestas, eget varius metus vehicula. Morbi sed sollicitudin odio. Curabitur interdum purus eget risus vestibulum consequat. Donec ornare, justo at luctus rhoncus, est quam ornare nulla, in congue nibh neque et magna.\nInteger laoreet sit amet nisl nec aliquam. Donec non euismod velit, eu laoreet justo. Suspendisse neque enim, finibus nec erat at, dictum tristique lorem. Nulla vitae augue vel mauris accumsan porta efficitur in purus. In tempor consectetur pellentesque. Mauris vitae quam eu ante varius consectetur. Suspendisse aliquam velit non enim pulvinar, nec hendrerit leo ultricies. Nam ornare lorem neque, quis dictum augue condimentum nec. Vivamus at dui tortor. Suspendisse a nunc dolor. Maecenas lobortis velit ac sapien ultricies, vel maximus ligula vulputate.\nAliquam eu urna non turpis semper pharetra eu a nisl. Sed eu nisi nec eros volutpat tristique vitae eget ex. Interdum et malesuada fames ac ante ipsum primis in faucibus. Aenean blandit auctor dolor nec pulvinar. Pellentesque sodales eros ac tellus consectetur, vel sollicitudin urna volutpat. Morbi at nulla et quam imperdiet dignissim. Sed vel lectus nec nisi condimentum semper tincidunt aliquam lorem. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.\nEtiam lobortis tellus at risus vehicula venenatis. Maecenas fermentum nunc felis, sed dictum risus dapibus at. Integer nulla libero, tempor at ullamcorper ut, facilisis fermentum purus. Aliquam et volutpat neque, et scelerisque dolor. Etiam nisl tortor, pellentesque eget nibh ac, accumsan placerat augue. Nulla consequat quam non nulla aliquet, ac placerat magna hendrerit. Ut massa nisi, lacinia venenatis lorem et, mattis finibus felis. Sed ipsum elit, convallis malesuada quam ac, sagittis interdum neque. Fusce a velit quis felis efficitur iaculis. Aenean sit amet aliquet nisl.\nSed porttitor pulvinar vulputate. Nam gravida iaculis nibh, vitae egestas lorem. Donec a aliquet massa, elementum pellentesque risus. Mauris imperdiet at risus nec elementum. Phasellus euismod neque tellus, sit amet convallis sem aliquam consequat. Vivamus ut ligula vitae magna scelerisque interdum. Vestibulum semper, ante ut pharetra malesuada, libero lorem mollis purus, sit amet sodales diam eros ac metus. In euismod erat sit amet consectetur tincidunt. ";

Window::Window(unsigned int screen_width, unsigned int screen_height, char *title, bool resizeable)
    : Width(screen_width), Height(screen_height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";

#endif
    glfwWindowHint(GLFW_RESIZABLE, resizeable);
    window = glfwCreateWindow(Width, Height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // ImGui configuration
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // io.Fonts->AddFontDefault();
    (void)io;
    // io.KeyMap[ImGuiKey_Backspace] = (GLFW_KEY_BACKSPACE);

    // Setup Dear ImGui style
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        success = false;
    }
    else
        success = true;

    if (success)
    {
        // OpenGL configuration
        // --------------------
        glViewport(0, 0, Width, Height);
        glEnable(GL_BLEND);
    }
    // setting up shaders to render each kind of element
    Shader shader = ResourceManager::LoadShader("./shaders/sprite.vs",
                                "./shaders/sprite.fs",
                                nullptr,
                                "sprite");

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, 1.0f, -1.0f);
    shader.Use().SetInteger("image", 0);
    shader.SetMat4("projection", projection);

    // set render-specific controls
    Renderer = new SpriteRenderer(shader);
    glViewport(0.0f, 0.0f, Width, Height); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION);           // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity();                      // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, Width, 0, Height, 1, -1);   // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW);            // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity();                      // same as above comment
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // setting the variables for the simulations!
    trackParticle = false;

    // load images for content
    ResourceManager::LoadTexture("./assets/A-LevelPics/cannonProjectileVect.png", true, "cannon2");
    ResourceManager::LoadTexture("./assets/A-LevelPics/kineticModelsOfMatter.png", true, "kineticModels");
    ResourceManager::LoadTexture("./assets/A-LevelPics/suvat1.png", true, "uvat");
    ResourceManager::LoadTexture("./assets/A-LevelPics/suvat2.png", true, "suat");
    ResourceManager::LoadTexture("./assets/A-LevelPics/suvat3.png", true, "suvt");
    ResourceManager::LoadTexture("./assets/A-LevelPics/suvat4.png", true, "suva");
    ResourceManager::LoadTexture("./assets/A-LevelPics/stateTempGraph.png", true, "stateTempGraph");
    ResourceManager::LoadTexture("./assets/A-LevelPics/Fig2VT.png", true, "Fig2VT");
    ResourceManager::LoadTexture("./assets/A-LevelPics/trapDoor.png", true, "trapDoor");
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    ImGui::StyleColorsLight();


}

void Window::setKeyCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(window, callback);
}

void Window::setFrameBufferSizeCallback(GLFWframebuffersizefun callback)
{
    glfwSetFramebufferSizeCallback(window, callback);
}

bool Window::gladInit()
{
    return success;
}

void Window::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::display()
{
    glfwSwapBuffers(window);
}

bool Window::isOpen()
{
    return !glfwWindowShouldClose(window);
}

void Window::Draw(Sprite sprite)
{
    Renderer->DrawSprite(sprite.Texture, sprite.Position, sprite.Size, sprite.Scale, sprite.Origin,
                         sprite.Rotation, sprite.Color);
}

void Window::Draw(RigidBody sprite)
{
    if (sprite.hasTexture)
        Renderer->DrawSprite(sprite.Texture, sprite.GetPosition(), sprite.Size, sprite.Scale, sprite.Origin,
                             sprite.Rotation, sprite.Color);
    else
    {
        Renderer->DrawShape(&sprite.Collider, sprite.GetPosition(), sprite.Color, sprite.Rotation);
    }
}

void Window::Draw(Texture2D texture, glm::vec2 size, glm::vec2 pos, float rotation)
{
    Renderer->DrawSprite(texture, pos, size, glm::vec2(1), glm::vec2(0),
                         rotation, glm::vec4(-1));
}

void Window::DrawLineSegment(glm::vec2 start, glm::vec2 end, float lineWidth, glm::vec4 colour)
{
    Renderer->DrawLine(start, end, lineWidth, colour);
}

void Window::DrawLine(glm::vec2 *path, int size, int linewidth, glm::vec4 colour, int start)
{
    
    for (int i = start + 1; i < start + size; i++)
    {
        if (path[i] != glm::vec2(-1))
            this->DrawLineSegment(path[(i - 1) % size], path[(i) % size], linewidth, colour);
        else
            break;
    }
}

void::Window::TopBar(){
    if (ImGui::BeginMainMenuBar()){
        if(ImGui::Button("Brownian Motion"))
            this->_state = BrownianBackground;

        if(ImGui::Button("Projectile Motion"))
            this->_state = ProjectileBackground;

        ImGui::EndMainMenuBar();
    }
}

void Window::DrawText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // TextRenderer->RenderText(text, x, y, scale, color);
}

void Window::BrownianControls(float *speed)
{
    ImVec2 static size = ImVec2(220, 100);
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), 0);
    ImGui::SetNextWindowSize(size, 0);
    ImGuiWindowFlags window_flags = 0;
    window_flags|=ImGuiWindowFlags_NoTitleBar;
    window_flags|=ImGuiWindowFlags_NoResize;
    ImGui::Begin("BrownianMotion", NULL, window_flags);

    char *button_txt;


    if (this->trackParticle)
        button_txt = "Click To Stop Tracking";
    else
        button_txt = "Click To Track!";

    if (ImGui::Button("Back"))
        this->_state = BrownianBackground;

    ImGui::SameLine();
    if (ImGui::Button(button_txt))
        this->trackParticle = !this->trackParticle;

//     size = ImGui::GetWindowSize();
    ImGui::TextWrapped("Use the slider below to increase/decrease the internal energy of the gas");
    ImGui::SliderFloat("##", speed, 0, 1);
    ImGui::End();
}
void Window::BrownianContext()
{
    ImVec2 static size = ImVec2(1080, 720);
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), 0);
    ImGui::SetNextWindowSize(size, 0);
    ImGui::SetNextWindowSizeConstraints(ImVec2(300.f, -1.f), ImVec2(INFINITY, -1.f));
    ImGuiWindowFlags window_flags = 0;

    window_flags |= ImGuiWindowFlags_NoTitleBar;
//     window_flags |= ImGuiWindowFlags_NoMove;
    window_flags|= ImGuiWindowFlags_NoResize;

    if (!ImGui::Begin("BrownianMotion", nullptr, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    auto center = 540 - (ImGui::CalcTextSize("About Brownian Motion!").x / 2);
    ImGui::SetCursorPosX(center);
    ImGui::SameLine(center); ImGui::Text("About Brownian Motion!");

    auto right = 1080 - (ImGui::CalcTextSize("Go To Simulation").x + 15);
    int borders = 190;
    ImGui::SameLine(right);
    if (ImGui::SmallButton("Go To Simulation"))
        this->_state = BrownianSim;
    ImGui::Separator();
    ImGui::Columns(3);
    ImGui::SetColumnWidth(-1,borders);
    ImGui::NextColumn();
    ImGui::SetColumnWidth(-1, size.x-borders*2);
    if (ImGui::CollapsingHeader("The Kinetic Model", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const char *KModel1 = R"(
The kinetic model describes how all substances are made up of atoms or molecules, which are arranged differently depending on the phase of the substance

In solids the atoms or molecules are regularly arranged and packed closely together, with strong electrostatic forces of attraction between them holding them in fixed positions, but they can vibrate and so have kinetic energy (Figure 2).

In liquids the atoms or molecules are still very close together, but they have more Kinetic energy than in solids, and — unlike in solids — they can change position and flow past each other.

In gases, the atoms or molecules have more kinetic energy again than those in liquids, and they are much further apart. They are free 10 move past each other as there are negligible electrostatic forces between them, unless they collide with each other or the container walls. They move randomly with different speeds in different directions.
)";
        ImGui::TextWrapped(KModel1);
        Texture2D image = ResourceManager::GetTexture("kineticModels");
        ImGui::SetCursorPos(ImVec2((size.x - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
        ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));

    }
    if (ImGui::CollapsingHeader("Observing Brownian Motion",ImGuiTreeNodeFlags_DefaultOpen))
    {
        const char*Motion1 = R"(The idea that substances were made of particles [atoms or molecules) was discussed for centuries, but not confirmed until 1827 when Robert Brown looked through a microscope and recorded his observations of the random movements of fine pollen grains floating on water.

It was not until 1905 that Albert Einstein fully explained this Brownian motion in terms of collisions between the pollen grains and millions of tiny water molecules. He explained that these collisions were elastic and resulted in a transfer of momentum from the water molecules to the pollen grains, causing the grains to move in haphazard ways. This provided the first significant proof of the kinetic model — the idea that matter is made up of atoms and molecules and they have kinetic energy. It is possible to observe Brownian motion in the laboratory using a smoke cell.
            )";
        ImGui::TextWrapped(Motion1);
    }
    if (ImGui::CollapsingHeader("Internal Energy and Root Mean Square Speed", ImGuiTreeNodeFlags_DefaultOpen))
    {
        para1 = R"(The internal energy of a substance is defined as:
    The sum of the randomly distributed kinetic and potential energies of
    atoms or molecules within the substance.

Consider a beaker of water at room temperature (Figure 2). The water contains a huge number of water molecules travelling at hundreds of meters per second. The internal energy of the water is the sum  of all the individual Kinetic energies of the water molecules in the glass and the sum of all the potential energies due to the electrostatic intermolecular forces between the molecules.

Now imagine cooling the beaker. The water will freeze and the water molecules move more slowly as the ice gets colder. Absolute zero is the lowest temperature possible. At this temperature the internal energy of a substance is a minimum. The Kinetic energy of all the atoms or molecules is zero — they have stopped moving. However, the internal energy is not zero because the substance still has electrostatic potential energy stored between the particles. Even at 0K, you cannot reduce the potential energy of the substance to zero.
)";
        ImGui::TextWrapped(para1);

        para1 = R"(Increasing the temperature of a body will increase its internal energy. As the temperature increases, the average kinetic energy of the atoms or molecules inside the body increases. In general, the hotter a substance, the faster the atoms or molecules that make up the substance move, and the greater the internal energy of the substance.

However, it is not only increasing the temperature of a body that increases its internal energy. When a substance changes phase, for example from solid to liquid, the temperature does not change, nor does the kinetic energy of the atoms or molecules. However, their electrostatic potential energy increases significantly.

1f a solid substance is heated using a heater with a constant power output, a graph showing how the temperature increases with time can be recorded (Figure 3).
)";
        ImGui::TextWrapped(para1);
        Texture2D image = ResourceManager::GetTexture("stateTempGraph");
        ImGui::SetCursorPos(ImVec2((size.x - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
        ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));

        para1 = R"(When a substance reaches its melting or boiling point, while it is changing phase the energy transferred to the substance does not
increase its temperature. Instead the electrostatic potential energy of the substance increases as the electrical forces between the atoms or molecules change. Only once the phase change is complete does the kinetic energy of the atoms or molecules increase further, and so the temperature rises again.

In different phases the atoms or molecules of a substance have different electrostatic potential energies:

    - Gas: The electrostatic potential energy is zero because there are negligible
        electrical forces between atoms or molecules.

    - Liquid: The electrostatic forces between atoms or molecules give the
        electrostatic potential energy a negative value. The negative simply means
        that energy must be supplied to break the atomic or molecular bonds.

    - Solid: The electrostatic forces between atoms or molecules are very large,
        so the electrostatic potential energy has a large negative value.

The electrostatic potential energy is lowest in solids, higher in liquids, and at its highest (0J) in gases.
)";
        ImGui::TextWrapped(para1);
    }

    ImGui::NextColumn();
    ImGui::SetColumnWidth(-1,borders);
    size = ImGui::GetWindowSize();
    ImGui::End();
}

void Window::ProjectileContext()
{
    ImVec2 static size = ImVec2(1080, 720);
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), 0);
    ImGui::SetNextWindowSize(size, 0);
    ImGui::SetNextWindowSizeConstraints(ImVec2(300.f, -1.f), ImVec2(INFINITY, -1.f));
    ImGuiWindowFlags window_flags = 0;
    int borders = 190;

    window_flags |= ImGuiWindowFlags_NoTitleBar;
//     window_flags |= ImGuiWindowFlags_NoMove;
    window_flags|= ImGuiWindowFlags_NoResize;

    if (!ImGui::Begin("BrownianMotion", nullptr, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    auto center = 540 - (ImGui::CalcTextSize("About Brownian Motion!").x / 2);
    ImGui::SetCursorPosX(center);
    ImGui::SameLine(center); ImGui::Text("About Projectile Motion!");
    int midColWidth = size.x-borders*2;
    auto right = 1080 - (ImGui::CalcTextSize("Go To Simulation").x + 15);
    ImGui::SameLine(right);
    if (ImGui::SmallButton("Go To Simulation"))
        this->_state = ProjectileSim;
    ImGui::Separator();
    ImGui::Columns(3);
    ImGui::SetColumnWidth(-1,borders);
    ImGui::NextColumn();
    ImGui::SetColumnWidth(-1, midColWidth);
    if (ImGui::CollapsingHeader("Equations of Motion"), ImGuiTreeNodeFlags_DefaultOpen){
        ImGui::TextWrapped(R"(              Predicting motion

We can use a knowledge of physics to predict the motion of accelerating or decelerating objects: for example, the impact speed of a small meteor about to hit the Earth, the initial speed of a car from skid marks left on the road, and the final speed of a space probe landing on a distant planet. It is amazing that all this is possible with just four equations.


                Equations of motion: the suvat equations

You need four equations to calculate quantities involving motion in a straight line at a constant acceleration. These equations of motion are often informally referred to as the ‘suvat equations’ after the symbols for the quantities involved.


                Deriving the equations of motion

Figure 2 shows the velocity-time graph for an accelerating object. The initial velocity of the object is u. After a time ¢ the final velocity of the object is v. The object has a constant acceleration a, as you can see from the straight-line graph. )");

        Texture2D fig = ResourceManager::GetTexture("Fig2VT");
        ImGui::SetCursorPos(ImVec2((size.x - fig.Width)*0.5f, ImGui::GetCursorPos().y));
        ImGui::Image((void*)(intptr_t)fig.ID, ImVec2(fig.Width, fig.Height));
        ImGui::TextWrapped("        This can be used to derive the following Equations");

        if (ImGui::BeginTable("suvat equations",4)){
            Texture2D image = ResourceManager::GetTexture("uvat");
    //         ImGui::SetCursorPos(ImVec2(borders + (midColWidth - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
            ImGui::TableNextColumn();ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));
            ImGui::TableNextColumn();

            image = ResourceManager::GetTexture("suat");
    //         ImGui::SetCursorPos(ImVec2((size.x/2 - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
            ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));
            ImGui::TableNextColumn();

            image = ResourceManager::GetTexture("suvt");
    //         ImGui::SetCursorPos(ImVec2(((3*size.x/2) - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
            ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));
            ImGui::TableNextColumn();

            image = ResourceManager::GetTexture("suva");
    //         ImGui::SetCursorPos(ImVec2(((3*size.x/2) - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
            ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));
            ImGui::EndTable();
        }

    }
    if (ImGui::CollapsingHeader("Freefall and g"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        ImGui::TextWrapped(R"()");
        Texture2D cannon = ResourceManager::GetTexture("trapDoor");
        ImGui::TextWrapped(R"(				From the edge of space

Felix Baumgartner made a record-breaking leap from the edge of space on 14th October 2012. A giant helium-filled balloon lifted his capsule 39.0km above the surface of the Earth. Then he stepped off. Baumgartner accelerated as he fell, reaching a maximum speed of 380ms~' — greater than the speed of sound — after just 50 seconds. He fell 36,4km in 4minutes 20seconds before deploying his parachute and landing safely just under 5minutes later.


				Acceleration due to gravity

Objects with mass exert a gravitational force on each other. The Earth is so massive that its gravitational pull is enough to keep us on its surface. An object released on the Earth will accelerate vertically downwards towards the centre of the Earth. When an object is accelerating under gravity, with no other force acting on it, it is said to be in free fall. The acceleration of free fall is denoted by the label g (not g, which means grams). Since g is an acceleration, it has the unit m/s^-2,


				Value for g close to Earth's surface

The value for g varies depending upon factors including altitude. latitude, and the geology of an area. For example, g is 9.825ms Helsinki, 9.816 ms in London, but only 9.776 ms? in Singapore. A value of 9.81 ms? is generally used.


				Determining g

The basic idea behind determining g in the laboratory is to drop a heavy ball over a known distance and time its descent. The problem is that it all happens very quickly, about 0.455 for a 1.0m fall. Methods for measuring g are described here.


               )");
    ImGui::TextWrapped(R"(				Electromagnet and trapdoor
An electromagnet holds a small steel ball above a trapdoor (Figure 2). When the current is switched off, a timer is triggered, the electromagnet demagnetises, and the ball falls. When it hits the trapdoor, the electrical contact is broken and the timer stops. The value for g is calculated from the height of the fall and the time taken.)");
    ImGui::SetCursorPos(ImVec2((size.x - cannon.Width)*0.5f, ImGui::GetCursorPos().y));
    ImGui::Image((void*)(intptr_t)cannon.ID, ImVec2(cannon.Width, cannon.Height));
    size = ImGui::GetWindowSize();
    ImGui::End();
    }
}

glm::vec2 Window::SandBox(int (*fire_projectile)(glm::vec2), void (*reset_projectile)(), phase state, glm::vec2 d)
{
    ImVec2 static size = ImVec2(313, 160);
    static glm::vec2 distance(0);
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, 720-160), 0);
    ImGui::SetNextWindowSize(size, 0);
    ImGui::SetNextWindowSizeConstraints(ImVec2(340.f, 160), ImVec2(400, 160));
    ImGuiWindowFlags window_flag2 = 0;
    window_flag2|=ImGuiWindowFlags_NoTitleBar;
    ImVec4 textGrey = ImVec4(0, 0, 0, .9f);
    ImGui::Begin("BrownianMotion", NULL, window_flag2);
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    static char x_axisbuf[64] = "0.0";
    static char y_axisbuf[64] = "0.0";
    ImGui::InputText("m/s x-axis", x_axisbuf, 64, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputText("m/s y-axis", y_axisbuf, 64, ImGuiInputTextFlags_CharsDecimal);
    static int fire = 0;
    if (state == READY)
    {
        distance = d;
    }
        char x_vec[16];
        char y_vec[16];
        sprintf(x_vec, "%.2f", distance.x/100);
        sprintf(y_vec, "%.2f", distance.y/100);
        ImGui::TextColored(ImVec4(.8f, 0, .8f, 1),"X-Displacement:");ImGui::SameLine();ImGui::TextColored(textGrey,x_vec); ImGui::SameLine();
        ImGui::TextColored(ImVec4(0,0,1,1),"Y-Displacement:");ImGui::SameLine();ImGui::TextColored(textGrey,y_vec);
    if (ImGui::Button("FireProjectile") && state == READY)
        fire++;
    if (fire && state == READY)
    {
        fire_projectile(glm::vec2(std::stof(x_axisbuf), -std::stof(y_axisbuf)));
        this->firedProjectile = true;
    }
    if (state == READY)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 1, 1), "READY TO GO!");
    }
    if (state == ACTIVE)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "FIRED");
    }
    if (state == ANIMATING)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "LOADING");
    }
    static int reset = 0;
    if (ImGui::Button("Reset") && (state != LOADING) && (state != ANIMATING))
        reset++;
        

    if (reset & 1)
    {
        reset_projectile();
        this->firedProjectile = false;
        if (fire > 0)
            fire--;
        reset--;
    }

    ImGui::Separator();
    if (ImGui::CollapsingHeader("Important Equations"))
    {
//         ImGui::TextWrapped(para1);
         ImGui::Columns(2);
        Texture2D image = ResourceManager::GetTexture("uvat");
        ImGui::SetCursorPos(ImVec2((size.x/2 - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
        ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));

        image = ResourceManager::GetTexture("suat");
        ImGui::SetCursorPos(ImVec2((size.x/2 - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
        ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));

        ImGui::NextColumn();
        image = ResourceManager::GetTexture("suvt");
        ImGui::SetCursorPos(ImVec2(((3*size.x/2) - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
        ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));

        image = ResourceManager::GetTexture("suva");
        ImGui::SetCursorPos(ImVec2(((3*size.x/2) - image.Width*.8)*0.5f, ImGui::GetCursorPos().y));
        ImGui::Image((void*)(intptr_t)image.ID, ImVec2(image.Width*.8, image.Height*.8));
    }

    size = ImGui::GetWindowSize();
    ImGui::End();
    float unverified_x = std::stof(x_axisbuf);
    float unverified_y = std::stof(y_axisbuf);
    float x_val = (unverified_x == 0) ? 0.01 : unverified_x;
    float y_val = (unverified_y == 0) ? 0.0001 : unverified_y;
    return glm::vec2(x_val, -y_val);
}
