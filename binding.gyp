{
  'targets': [
    {
      'target_name': 'tree_sitter_compat',
      'sources': [ 'src/compat.cc' ],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except",
      ],
      'defines': [ 'NAPI_VERSION=8' ],
      'conditions': [
        ['OS=="mac"', {
            'cflags+': ['-fvisibility=hidden'],
            'xcode_settings': {
              'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
            }
        }]
      ]
    },
  ]
}
