/** @type {import('next').NextConfig} */
const nextConfig = {
    webpack: (config) => {
      config.module.rules.push({
        test: /\.mp3$/,
        use: {
          loader: 'file-loader',
          options: {
            name: '[name].[ext]',
            outputPath: 'static/media',
            publicPath: '/_next/static/media',
          },
        },
      });
  
      return config;
    },
  };
  
  export default nextConfig;