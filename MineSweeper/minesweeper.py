#!/usr/bin/env python3
import numpy as np

class Minesweeper():
	def __init__(self, board_size, num_bombs):
		# -1 = bomb cell, 0 = empty cell, positive integer = number of neighboring bombs
		self.shadow_board = np.zeros((board_size, board_size), dtype=int)
		self.board_size = board_size
		self.num_bombs = num_bombs

		empty_cells = [np.array([x,y]) for x in range(board_size) for y in range(board_size)]
		
		# Initialize bombs to a maximum of 2 * board_size - 1
		for x in range(min(num_bombs, board_size * 2 - 1)):
			pos = np.random.randint(len(empty_cells))
			coord = empty_cells[pos]
			self.shadow_board[coord[0]][coord[1]] = -1

			# Initialize bomb neighbor cells
			for c in self.get_neighbor_coords(coord):
				if not self.exceed_board_space(c):
					if self.shadow_board[c[0], c[1]] != -1:
						self.shadow_board[c[0], c[1]] += 1

			del empty_cells[pos]

		# The board shown to the player, 1 = opened by the player, 0 = still remain closed
		self.player_board = np.zeros((board_size, board_size), dtype=int)

	# Return True if game has ended, False otherwise
	def make_move(self, coord):
		if self.shadow_board[coord[0], coord[1]] == -1:
			# If this cell is a bomb, end game
			print("PLAYER HAS LOST!!! IT IS A BOMB")
			self.player_board = np.ones((board_size, board_size), dtype=int)
			return True
		elif self.shadow_board[coord[0], coord[1]] == 0:
			# If this cell is empty
			self.player_board[coord[0], coord[1]] = 1

			# Make shadow move on neighbor
			for c in self.get_neighbor_coords(coord):
				if not self.exceed_board_space(c):
					self.make_shadow_move(c)
		else:
			# If this cell has a bomb neighbor
			self.player_board[coord[0], coord[1]] = 1

		# If player has won the game, end game
		if self.has_win_game():
			print("PLAYER HAS WON!!!!")
			self.player_board = np.ones((board_size, board_size), dtype=int)
			return True

	# Make move recursively and ignore bombs, use to reveal the board
	# Algorithm:
	# If the cell is already revealed, just do nothing
	# If the cell is empty, reveal the cell and call this function recursively on its neighbors
	# If the cell is the neighbor of the bomb, reveal that cell but don't call this function again
	# If the cell is a bomb, do nothing
	def make_shadow_move(self, coord):
		if not self.player_board[coord[0], coord[1]]:
			# If it is not revealed yet
			if self.shadow_board[coord[0], coord[1]] == 0:
				self.player_board[coord[0], coord[1]] = 1
				# Make shadow move on neighbor
				for c in self.get_neighbor_coords(coord):
					if not self.exceed_board_space(c):
						self.make_shadow_move(c)
			elif self.shadow_board[coord[0], coord[1]] != -1:
				# If it is the neighbor of the bomb
				self.player_board[coord[0], coord[1]] = 1
		else:
			print("This cell is already revealed")

	# Return true if coord exceed board limit
	def exceed_board_space(self, coord):
		return coord[0] < 0 or coord[0] >= self.board_size or coord[1] < 0 or coord[1] >= self.board_size

	# Return neighbor coordinates without checking if they exceed board limit
	def get_neighbor_coords(self, coord):
		vectors = np.array([[-1,-1], [-1,0], [-1,1], [0,-1], [0,1], [1,-1], [1,0], [1,1]])

		return vectors + coord

	# Return true if player has win the game
	def has_win_game(self):
		return np.sum(self.player_board) >= self.board_size * self.board_size - self.num_bombs

	def __str__(self):
		ar = self.shadow_board * self.player_board
		sep = '----|' * (self.board_size + 1)
		rows = []

		rows.append('    |' + ''.join(['%4d|' % (t) for t in range(self.board_size)]))
		rows.append(sep)

		for i, row in enumerate(ar):
			rows.append('%4d|' % (i) + '|'.join(['%4d' % t for t in row]))
			rows.append(sep)

		return '\n'.join(rows)

print("Minesweeper Game\nPlease input board size: ", end='')
board_size = int(input())
print("Please input number of bombs: ", end='')
num_bombs = int(input())

game = Minesweeper(board_size, num_bombs)

print(game)

while True:
	print("\nPlease insert coordinates (x y): ", end='')
	coord = [int(x) for x in input().split()]
	if game.make_move(coord):
		break
	print(game)
	print()
print(game)
print()