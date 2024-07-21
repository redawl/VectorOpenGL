.section .note.GNU-stack,"",@progbits
.global shader_fs
    .global shader_fs_size
    .section .rodata
shader_fs:
    .incbin "src/VectorShaders/shader.fs"
1:
shader_fs_size:
    .int 1b - shader_fs

