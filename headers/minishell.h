/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 09:00:35 by abahja            #+#    #+#             */
/*   Updated: 2025/06/05 21:52:39 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <errno.h>
# include <sys/stat.h>
# include <signal.h>

typedef enum s_type
{
	WORD,
	RED_IN,
	RED_OUT,
	APPEND,
	PIPE,
	OR,
	S_QWORD,
	D_QWORD,
	AND,
	LPAREN,
	S_COL,
	RPAREN,
	HEREDOC,
	W_SPACE,
	DEL,
	Q_DEL,
	W_CHAR,
	NILL,
}	t_type;

typedef struct sig_communication
{
	char	is_heredoc;
	char	is_excution;
	char	is_child;
}	t_sig;

typedef struct env_copy
{
	char			*key;
	char			*value;
	struct env_copy	*next;
}	t_env;

typedef struct s_heapdata
{
	void				*ptr_h;
	struct s_heapdata	*next;
}	t_heapdata;

typedef struct s_re
{
	char		q_del;
	char		f_ambiguous;
	t_type		type;
	int			hd_fd;
	char		*filename;
	char		*del;
	struct s_re	*next;
}	t_re;

typedef struct s_eflow
{
	char			is_built_in;
	t_re			*redirs;
	t_re			*tmp;
	char			*cmd;
	char			**arg;
	struct s_eflow	*next;
	struct s_eflow	*prev;
}	t_eflow;

typedef struct s_token
{
	char			visited;
	char			ambiguous;
	char			expanded;
	char			assign;
	t_type			type;
	char			*bin;
	char			*brick;
	char			*old;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct minishell
{
	t_type		t_del[3];
	t_type		t_word[4];
	t_type		t_redirs[5];
	t_type		t_nohandle[6];
	char		*oldpwd;
	char		*prompt;
	t_eflow		*flow;
	t_token		*token;
	t_heapdata	*heapdata;
	t_env		*env;
	char		**env_arr;
}	t_minishell;

void		name_file(t_type type, t_token *token, t_re *re);
void		no_need(t_eflow *flow);
void		ft_skip2(t_re **head, t_re *tmp);
void		copy_ambiguous(t_re *re, int amb);
void		set_env(t_eflow *flow, t_minishell *bash, int f, int i);
void		update(t_minishell *bash, char *curr, char *old);
char		**env_setup(t_minishell *bash);
char		*stdin_prompt(void);
void		*heap_manager(int size, char flag, void *to_free);
t_heapdata	*ft_lstlast(t_heapdata *lst);
void		ft_lstadd_back(t_heapdata **lst, t_heapdata *new);
void		lst_add_back(t_token **head, t_token *new);
int			syntax(t_minishell *bash);
t_token		*create_new(int size, t_type type);
void		re_add_new(t_re **redirs, t_re *nredir);
t_re		*create_re(t_type type, t_token *token);
void		flow_add_new(t_eflow **flow, t_eflow *nflow);
t_eflow		*create_flow(t_re *redirs);
void		make_flow(t_minishell *bash, t_token *t, t_re *redir, t_type type);
void		expandator(t_minishell *bash, int f);
int			is_in_token_list(t_type type, t_type *list);
void		ft_spacer(t_minishell *bash);
char		*get_value(t_env *env, char *s, char check, t_token *t);
void		is_builtin(t_eflow *target);
void		exit_bash(char **arg);
void		execution_trigger(t_minishell *bash, t_eflow *flow, int f);
char		*path_finder(t_eflow *flow, char **env, int error);
t_type		ft_ismeta(char *str, int *i, int *flag);
int			is_redir(t_token *token);
t_eflow		*ft_last(t_eflow *last);

int			exit_status(int status);
int			arg_count(char **args);
int			add_exist(t_env *env, char *str);
int			set_redirections(t_eflow *flowblock);
void		pwd(t_minishell *bash);
void		echo(char **args);
void		env(char **env, char **arg);
void		export(char **arg, t_env *env, char **env_arr, int update);
void		cd(char **args, t_minishell *bash);
void		unset(t_minishell *bash, char **args);
void		minishell(t_minishell *bash);
pid_t		execute_cmd(t_eflow *flow, t_minishell *bash,
				int prev_fd, int *fds);
void		one_cmd(t_minishell *bash);
char		*ft_envsubstr(char const *s, unsigned int start, size_t len);
char		*ft_envstrdup(const char *s);
char		*ft_envstrjoin(char const *s1, char const *s2);
char		*ft_pathjoin(char const *s1, char const *s2);
void		final_label(long long exit_code);
t_sig		*state(void);
void		handle_doc(int sig);
void		check_fail(int returned);
void		clear_quotes(t_token *token);
void		check_ambiguous(t_token *t, char *s, int expanded);
void		simple_mode(t_token *new, char *slice, int size);

void		list_removeif(t_env **env, char *key);

void		init_pwd(t_minishell *bash);
/*---------------------bash_setup---------------------------*/
void		init_bash(t_minishell *bash, int *inods);
/*made for backup fd from stdin & stdout if fails exit the caller process*/
void		fd_backup(int *stdin_backup, int *stdout_backup, char exit);
/*--set the default reactions of the ft_bash--*/
void		bash_signal(int sig);
/*-------------------env_setup-----------------------*/

/*----made for copying from environ[spesific index] \n offset to a env node--*/
t_env		*create(char *s);
/*---------append to a list of env -------*/
void		add_back(t_env **head, t_env *new);
/*-------------setup the first call env----------------*/
t_env		*copy_env(void);

/*------------------bash_core--------------------------*/
void		the_core(t_minishell *bash);
void		core_tokinizer(t_minishell *bash);
int			quotes(char *s);

/*----------------------here_doc----*/
int			here_doc(t_minishell *bash);

char		*env_path(char **env);
void		pipe_cmd(t_minishell *bash, int status, int prev_fd);
void		execve_fail(char *cmd, char **env);
pid_t		forking(void);
char		ft_errno(char err);
void		open_handler(int fd);
int			last_open_fd(int fd);
void		clear_cache(t_eflow *flows);
int			expandable(const char *s, int i, char *ss);

#endif