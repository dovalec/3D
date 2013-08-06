//
//  Shader.fsh
//  EngineTest
//
//  Created by Dov Caspi on 8/5/13.
//  Copyright (c) 2013 Dov Caspi. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
