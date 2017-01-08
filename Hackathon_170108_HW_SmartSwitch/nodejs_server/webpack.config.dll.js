const path = require('path'); //处理路径
const merge = require('webpack-merge'); //合并配置项
const validator = require('webpack-validator'); //验证配置项是否合理
const HtmlWebpackPlugin = require('html-webpack-plugin'); //生成html
const webpack = require('webpack');

//设置加载路径
const PATHS = {
    test: path.resolve(__dirname, "text1.jsx"),
    app: path.resolve(__dirname, "app", "index.jsx"),
    build: path.resolve(__dirname, "build"),
    favicon: path.resolve(__dirname, "app/src/favicon.png"),
    style: [
        path.resolve(__dirname, 'app/src/antdFont.css'),
        path.resolve(__dirname, 'app/src/flex.css'),
        path.resolve(__dirname, 'node_modules/antd/dist/antd.css')
    ]
};

//加载其他配置项
const parts = require('./libs/parts');
//公共配置项
const common_config = {
    entry: {
        polyfill: "babel-polyfill",
        app: PATHS.app,
        style: PATHS.style
    },
    output: {

        path: PATHS.build,
        filename: "[name].js"
    },
    plugins: process.env.NODE_ENV === 'production' ? [
        /* new webpack.optimize.DedupePlugin(),
         new webpack.optimize.OccurrenceOrderPlugin(),
         new webpack.optimize.UglifyJsPlugin(),*/
        new HtmlWebpackPlugin({
            template: require('html-webpack-template'), //生成模版html
            title: "鹰视利|我们的眼护专家",
            appMountId: "app",
            inject: false,
            favicon: PATHS.favicon
        })
    ] : [new webpack.optimize.UglifyJsPlugin(),
        new HtmlWebpackPlugin({
            template: require('html-webpack-template'), //生成模版html
            title: "鹰视利|我们的眼护专家",
            appMountId: "app",
            inject: false,
            favicon: PATHS.favicon
        })],

    module: {
        loaders: [{
            test: /\.(js|jsx)$/,
            loader: "babel",
            query: {
                presets: ["es2015", "stage-2", "react"]
            },
            exclude: /node_modules/
        },

            {
                test: /\.(jpg|png)$/,
                loader: "file?name=images/[name].[ext]"

            }
        ]
    }

};
let config;
//中间件是讲顺序的
switch (process.env.npm_lifecycle_event) {
    case "dll":
        config = merge(common_config, {
                devtool: 'source-map',
                output: {
                    path: PATHS.build,
                    filename: '[name].[chunkhash].js',
                    chunkFilename: '[chunkhash].js'
                }
            },
            parts.clean(PATHS.build),
            parts.extractCSS(PATHS.style)
        );
        break;
    case "dev":
        config = merge(common_config,
            {
                devtool: 'eval-source-map'
            },
            parts.devServer({
                // Customize host/port here if needed
                host: process.env.HOST,
                port: process.env.PORT
            }),
            parts.clean(PATHS.build),
            parts.extractCSS(PATHS.style)
        );
        break;

    default:
        config = merge(
            common_config,
            {
                devtool: 'eval-source-map'
            },
            parts.setupCSS(PATHS.style),
            parts.devServer({
                // Customize host/port here if needed
                host: process.env.HOST,
                port: process.env.PORT
            })
        );
}

module.exports = validator(config);
