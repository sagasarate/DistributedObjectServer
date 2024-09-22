const gulp = require('gulp');
const rollup = require('rollup');
const ts = require('gulp-typescript');
const sourcemaps = require('gulp-sourcemaps');
const resolve = require('@rollup/plugin-node-resolve');
const commonjs = require('@rollup/plugin-commonjs');
const gulp_sourcemaps = require('rollup-plugin-sourcemaps');

async function getBom() {
    return (await import('gulp-bom')).default;
}

const tsProject = ts.createProject('tsconfig.json', { declaration: true });

const onwarn = warning => {
    if (warning.code === 'CIRCULAR_DEPENDENCY')
        return;
    console.warn(`(!) ${warning.message}`);
};

// 编译 TypeScript 并生成 source map
gulp.task('buildJs', () => {
    return tsProject.src()
        .pipe(sourcemaps.init()) // 初始化 source map
        .pipe(tsProject())
        .pipe(sourcemaps.write('./', {
            sourceRoot: '../src'  // 设置 source map 文件中源文件的路径
        }))
        .pipe(gulp.dest('./build'));
});

// 使用 Rollup 打包并生成 Source Map
gulp.task("rollup", async function () {
    const bundle = await rollup.rollup({
        input: "build/Main.js",
        onwarn,  // 添加警告处理
        plugins: [
            resolve({
                preferBuiltins: true, // 优先使用 Node.js 内置模块
            }),
            commonjs(),
            gulp_sourcemaps()
        ],  // 这里可以添加 Rollup 插件

    });

    await bundle.write({
        file: 'out/Main.mjs',
        format: 'esm',
        sourcemap: true  // 启用 source map 生成        
        //banner: "require('source-map-support').install();\n", // 在文件头部插入
    });
});

// 为输出文件添加 BOM
gulp.task('addBom', async () => {
    const bom = await getBom(); // 动态导入

    return gulp.src('out/Main.mjs', 'out/Main.mjs.map')
        .pipe(bom()) // 添加 BOM
        .pipe(gulp.dest('out'));
});

// 构建任务顺序
gulp.task('build', gulp.series(
    'buildJs',
    'rollup',
    'addBom',
));
