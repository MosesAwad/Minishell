NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

SRCS = main.c binlist_setup.c
RDLINE_L = -L/Users/$(USER)/.brew/opt/readline/lib -lreadline

TOK_DIR = Tokenization
TOK_SRCS = $(addprefix $(TOK_DIR)/, check_toklist_quotes.c token.c get_next_line.c general_case_handler.c token_utils.c)

PARSE_DIR = Parser
PARSE_SRCS = $(addprefix $(PARSE_DIR)/, 1-cmdline_level.c 2-job_level.c 3-1-command_level.c 3-2-command_level_utils.c 4-redir_list_level.c 5-1-simple_command_level.c 5-2-simple_command_level_utils.c 6-arg_list_level.c ASTree_utils.c parse_setup.c)

EXPAND_DIR = Expander
EXPAND_SRCS = $(addprefix $(EXPAND_DIR)/, expand_str_or_toklist.c expander_cases1.c expander_cases2.c expander_cases3.c expander_utils.c expander_utils2.c expander.c strip_quotes.c)

EXEC_DIR = Execution
EXEC_SRCS = $(addprefix $(EXEC_DIR)/, file_dups.c count_procs.c init_zone.c free_zone.c execution1.c execution2.c execution_utils.c files_setup.c redir_handling.c hdoc.c redir_handling_utils.c file_handling.c)

COMMAND_DIR = Command
COMMAND_SRCS = $(addprefix $(COMMAND_DIR)/, set_up_command.c command_utils.c \
					Cd/cd.c \
					Export/export_w_flags_utils.c Export/export_w_flags.c Export/export_no_flags.c Export/export.c \
					Env/env.c \
					Unset/unset.c \
					Pwd/pwd.c \
					Echo/echo.c \
					Exit/exit.c)

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJS = $(SRCS:.c=.o)
TOK_OBJS = $(TOK_SRCS:.c=.o)
PARSE_OBJS = $(PARSE_SRCS:.c=.o)
EXPAND_OBJS = $(EXPAND_SRCS:.c=.o)
EXEC_OBJS = $(EXEC_SRCS:.c=.o)
COMMAND_OBJS = $(COMMAND_SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

#$(LIBFT) has to be the last one in the statement below
#-g3 -fsanitize=address
$(NAME): $(OBJS) $(TOK_OBJS) $(PARSE_OBJS) $(EXPAND_OBJS) $(EXEC_OBJS) $(COMMAND_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(TOK_OBJS) $(PARSE_OBJS) $(EXPAND_OBJS) $(EXEC_OBJS) $(COMMAND_OBJS) $(LIBFT) $(RDLINE_L)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(TOK_OBJS) $(PARSE_OBJS) $(EXPAND_OBJS) $(EXEC_OBJS) $(COMMAND_OBJS)


fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)


re: fclean all


rebonus: fclean bonus