MRuby::Gem::Specification.new('mruby-zmq') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Yoshikawa Ryota'
  spec.cc.include_paths << '/usr/local/Cellar/zeromq/4.0.4/include/zmq.h'
  spec.linker.libraries << ['zmq']
end
