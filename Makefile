# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/21 11:57:08 by msoriano          #+#    #+#              #
#    Updated: 2026/09/21 13:56:58 by msoriano         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= libftpp.a

CXX				:= c++
CXXFLAGS		:= -Wall -Wextra -Werror -std=c++17
AR				:= ar rcs
RM				:= rm -rf

# ──────────────────────────────────────────────────────────────────────────────
# DIRECTORIES
# ──────────────────────────────────────────────────────────────────────────────
OBJ_DIR			:= .obj

# ──────────────────────────────────────────────────────────────────────────────
# SOURCES
# ──────────────────────────────────────────────────────────────────────────────
SRCS			:= \
	structs/data_buffer.cpp \
	patterns/memento.cpp \
	iostream/thread_safe_iostream.cpp \
	threading/thread.cpp \
	threading/worker_pool.cpp \
	threading/persistent_worker.cpp

OBJS			:= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# ──────────────────────────────────────────────────────────────────────────────
# COLORS
# ──────────────────────────────────────────────────────────────────────────────
RESET			:= \033[0m
BOLD			:= \033[1m
GREEN			:= \033[32m
CYAN			:= \033[36m
YELLOW			:= \033[33m
RED				:= \033[31m
PURPLE			:= \033[35m

# ──────────────────────────────────────────────────────────────────────────────
# RULES
# ──────────────────────────────────────────────────────────────────────────────
all: $(NAME)

$(NAME): $(OBJS)
	@echo ""
	@echo "$(PURPLE)$(BOLD)╔══════════════════════════════════════╗$(RESET)"
	@echo "$(PURPLE)$(BOLD)║          BUILDING LIBFTPP            ║$(RESET)"
	@echo "$(PURPLE)$(BOLD)╚══════════════════════════════════════╝$(RESET)"
	@$(AR) $(NAME) $(OBJS)
	@echo "$(GREEN)✓ $(NAME) successfully created$(RESET)"
	@echo ""

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "$(CYAN)→ Compiling$(RESET) $(YELLOW)$<$(RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(RED)→ Removing object files...$(RESET)"
	@$(RM) $(OBJ_DIR)
	@echo "$(GREEN)✓ Objects cleaned$(RESET)"

fclean: clean
	@echo "$(RED)→ Removing $(NAME)...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)✓ Library cleaned$(RESET)"

re: fclean all

.PHONY: all clean fclean re