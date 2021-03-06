""" oc_pattern 

This module contains a collection of YANG definitions
for sanity package.

This module contains definitions
for the following management objects\:


Copyright (c) 2013\-2014 by Cisco Systems, Inc.
All rights reserved.

"""


import re
import collections

from enum import Enum

from ydk.types import Empty, YList, YLeafList, DELETE, Decimal64, FixedBitsDict

from ydk.errors import YPYError, YPYModelError




class OcA(object):
    """
    
    
    .. attribute:: a  <key>
    
    	blah
    	**type**\:  str
    
    	**refers to**\: :py:class:`b <ydk.models.oc_pattern.OcA.B>`
    
    .. attribute:: b
    
    	
    	**type**\:  :py:class:`B <ydk.models.oc_pattern.OcA.B>`
    
    

    """

    _prefix = 'oc'
    _revision = '2015-11-17'

    def __init__(self):
        self.a = None
        self.b = OcA.B()
        self.b.parent = self


    class B(object):
        """
        
        
        .. attribute:: b
        
        	
        	**type**\:  str
        
        

        """

        _prefix = 'oc'
        _revision = '2015-11-17'

        def __init__(self):
            self.parent = None
            self.b = None

        @property
        def _common_path(self):
            if self.parent is None:
                raise YPYModelError('parent is not set . Cannot derive path.')

            return self.parent._common_path +'/oc-pattern:B'

        def is_config(self):
            ''' Returns True if this instance represents config data else returns False '''
            return True

        def _has_data(self):
            if not self.is_config():
                return False
            if self.b is not None:
                return True

            return False

        @staticmethod
        def _meta_info():
            from ydk.models._meta import _oc_pattern as meta
            return meta._meta_table['OcA.B']['meta_info']

    @property
    def _common_path(self):
        if self.a is None:
            raise YPYModelError('Key property a is None')

        return '/oc-pattern:oc-A[oc-pattern:a = ' + str(self.a) + ']'

    def is_config(self):
        ''' Returns True if this instance represents config data else returns False '''
        return True

    def _has_data(self):
        if not self.is_config():
            return False
        if self.a is not None:
            return True

        if self.b is not None and self.b._has_data():
            return True

        return False

    @staticmethod
    def _meta_info():
        from ydk.models._meta import _oc_pattern as meta
        return meta._meta_table['OcA']['meta_info']


