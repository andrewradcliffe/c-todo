#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <leif/leif.h>

typedef enum
{
  ALL = 0,
  IN_PROGRESS,
  COMPLETED,
  LOW,
  MEDIUM,
  HIGH
} EntryFilter;

#define WIN_MARGIN 20.0f

static int winw = 1280, winh = 720;
static LfFont title_font;
static EntryFilter current_filter;

static void renderTopBar()
{
  lf_push_font(&title_font);
  lf_text("Your To Do");
  lf_pop_font();

  {
    const float width = 160.0f;

    lf_set_ptr_x_absolute(winw - width - WIN_MARGIN * 2.0f);
    LfUIElementProps props = lf_get_theme().button_props;
    props.margin_left = 0.0f; props.margin_right = 0.0f;
    props.color = (LfColor){65, 167, 204, 255};
    props.border_width = 0.0f; props.corner_radius = 4.0f; 
    lf_push_style_props(props);
    lf_set_line_should_overflow(false);
    lf_button_fixed("New Task", 160, -1);
    lf_set_line_should_overflow(true);
    lf_pop_style_props();
  }
}

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(winw, winh, "Todo", NULL, NULL);

	glfwMakeContextCurrent(window);

  lf_init_glfw(winw, winh, window);
  LfTheme theme = lf_get_theme();
  theme.div_props.color = LF_NO_COLOR;
  lf_set_theme(theme);

  title_font = lf_load_font("./fonts/inter-bold.ttf", 40);

	while(!glfwWindowShouldClose(window)) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    lf_begin();

    lf_div_begin(((vec2s){WIN_MARGIN, WIN_MARGIN}), 
                 ((vec2s){winw - WIN_MARGIN * 2.0f, winh - WIN_MARGIN * 2.0f}),
                 true);
    
    renderTopBar();
    
    lf_next_line();

    {
      const uint32_t num_filters = 6;
      static const char* filters[] = {"ALL", "IN PROGRESS", "COMPLETED", "LOW", "MEDIUM", "HIGH"};

      LfUIElementProps props = lf_get_theme().text_props;
      props.margin_top = 30.0f;
      props.margin_right = 30.0f;
      props.padding = 12.0f;
      props.corner_radius = 2.0f;

      float width = 0.0f;
      float ptrx_before = lf_get_ptr_x();
      float ptry_before = lf_get_ptr_y();
      lf_push_style_props(props);
      lf_set_no_render(true);
      lf_set_ptr_y_absolute(lf_get_ptr_y() + 50.0f);
      for (uint32_t i = 0; i < num_filters; i++)
      {
        lf_button(filters[i]);
      }
      lf_set_no_render(false);
      lf_set_ptr_y_absolute(ptry_before);

      width = lf_get_ptr_x() - ptrx_before - props.margin_right - props.padding;

      lf_set_ptr_x_absolute(winw - width - WIN_MARGIN);
      
      lf_set_line_should_overflow(false);
      for (uint32_t i = 0; i < num_filters; i++)
      {
        props.color = (current_filter == i) ? (LfColor){120, 120, 120, 255} : LF_NO_COLOR;
        lf_push_style_props(props);
        if (lf_button(filters[i]) == LF_CLICKED)
        {
          current_filter = (EntryFilter)i;
        };
        lf_pop_style_props();
      }
      lf_set_line_should_overflow(true);
    }

    lf_div_end();
    lf_end();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
  
  lf_free_font(&title_font);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
