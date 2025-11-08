# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/30 10:15:34 by abahja            #+#    #+#              #
#    Updated: 2025/06/05 21:51:42 by mochajou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC      = cc
CFLAGS  = -g -Werror -Wextra -Wall #-fsanitize=address
NAME    = minishell

SRC = minishell.c srcs/setup_bash.c srcs/setup_env.c bash_core/core_bash.c bash_core/flow_bash.c heredoc_bash/here_doc.c heredoc_bash/here_doc_utils.c ft_dllman.c bash_core/syntax_layer.c listutils.c \
	  bash_core/build_flow.c expandator.c bash_core/remove_space.c execution/excut_label.c builtins/pwd.c builtins/echo.c builtins/cd.c  builtins/env.c  builtins/exit.c\
	  builtins/export.c builtins/unset.c execution/execution.c execution/execute.c fix_env/env_fix.c little_helpers.c execution/cmd_path.c bash_core/utils_token.c signal_handlers/sighandler.c\
	  execution/cmd_utils.c

OBJ     = $(SRC:.c=.o)

LIBS_DIR = libft
LIBFT_LINK = -L $(LIBS_DIR) -lft
LIBRL_LINK = -L $(LIBS_DIR) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	make -C $(LIBS_DIR)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_LINK) $(LIBRL_LINK) -o $(NAME)
	@echo "minshell compiled successfully!"

.SECONDARY: $(OBJ)

clean:
	@rm -f $(OBJ)
	make -C $(LIBS_DIR) clean
	@echo "Cleaned object files!"

fclean: clean
	make -C $(LIBS_DIR) fclean
	@rm -f $(NAME)
	@echo "Removed executable!"

re: fclean $(NAME)

.PHONY: all clean fclean re
