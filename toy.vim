source ~/.config/nvim/init.vim
set runtimepath+=./
autocmd TextChanged,TextChangedI *.glsl lua require"toy".send_code()
" nvim -u toy.vim fragment.glsl
